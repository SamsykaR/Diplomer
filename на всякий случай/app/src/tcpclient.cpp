#include "tcpclient.h"
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkInterface>
#include <QDebug>
#include <QCoreApplication>
#include <QTimer>

TcpClient::TcpClient(Logs *logger, QObject *parent)
    : QObject(parent), socket(nullptr), nextRequestId(1), userId(-1), loger(logger)
{
    loger->info("TcpClient: создан");
}

TcpClient::~TcpClient()
{
    loger->info("TcpClient: уничтожение");
    disconnectFromServer();
}

bool TcpClient::connectToServer(const QString &host, quint16 port)
{
    loger->info("TcpClient: попытка подключения к " + host + ":" + QString::number(port));
    try {
        if (socket) disconnectFromServer();
        socket = new QTcpSocket(this);
        socket->connectToHost(host, port);
        if (!socket->waitForConnected(3000))
            throw NetworkEx("Ошибка подключения: " + socket->errorString());

        loger->info("TcpClient: подключено к серверу");

        connect(socket, &QTcpSocket::disconnected, this, [this]() {
            loger->error("TcpClient: соединение с сервером разорвано");
            emit errorOccurred("Соединение с сервером потеряно. Проверьте сервер и перезапустите приложение.");
        });

        connect(socket, &QTcpSocket::readyRead, this, [this]() {
            QByteArray newData = socket->readAll();
            loger->info("TcpClient: получено " + QString::number(newData.size()) + " байт");
            buffer.append(newData);
            while (true) {
                int nl = buffer.indexOf('\n');
                if (nl == -1) break;
                QByteArray line = buffer.left(nl);
                buffer.remove(0, nl + 1);
                QJsonParseError err;
                QJsonDocument doc = QJsonDocument::fromJson(line, &err);
                if (err.error != QJsonParseError::NoError) {
                    loger->warning("TcpClient: ошибка парсинга JSON: " + err.errorString());
                    continue;
                }
                if (!doc.isObject()) continue;
                QJsonObject obj = doc.object();
                if (obj.contains("response")) {
                    int reqId = obj.value("requestId").toInt();
                    bool success = obj.value("response").toBool();
                    QVariantMap data = obj.value("data").toObject().toVariantMap();
                    data["_success"] = success;
                    pendingResponses[reqId] = data;
                    emit responseReceived(reqId, success, data);
                    loger->info("TcpClient: получен ответ на запрос " + QString::number(reqId) + ", успех=" + QString(success ? "true" : "false"));
                } else if (obj.contains("notification")) {
                    QString type = obj.value("notification").toString();
                    loger->info("TcpClient: получено уведомление: " + type);
                    if (type == "data_changed")
                        emit dataChanged();
                }
            }
        });
        connect(socket, &QTcpSocket::disconnected, this, &TcpClient::disconnectFromServer);
        return true;
    } catch (const NetworkEx &e) {
        loger->error(QString("connectToServer: ") + e.what());
        emit errorOccurred(e.what());
        if (socket) {
            socket->deleteLater();
            socket = nullptr;
        }
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("connectToServer: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка подключения к серверу");
        return false;
    }
}

void TcpClient::disconnectFromServer()
{
    loger->info("TcpClient: отключение от сервера");
    if (socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
        socket = nullptr;
    }
    buffer.clear();
    pendingResponses.clear();
}

bool TcpClient::isConnected() const
{
    return socket && socket->state() == QAbstractSocket::ConnectedState;
}

int TcpClient::sendRequest(const QString &command, const QVariantMap &data)
{
    loger->info("TcpClient: отправка запроса '" + command + "'");
    if (!isConnected())
        throw NetworkEx("Нет соединения с сервером");

    int reqId = nextRequestId++;
    QJsonObject obj;
    obj["command"] = command;
    obj["requestId"] = reqId;
    if (!data.isEmpty())
        obj["data"] = QJsonObject::fromVariantMap(data);
    QByteArray msg = QJsonDocument(obj).toJson(QJsonDocument::Compact) + "\n";
    qint64 bytes = socket->write(msg);
    if (bytes == -1)
        throw NetworkEx("Ошибка записи в сокет: " + socket->errorString());
    socket->flush();
    loger->info("TcpClient: запрос " + QString::number(reqId) + " отправлен, байт: " + QString::number(bytes));
    return reqId;
}

bool TcpClient::waitForResponse(int requestId, QVariantMap &resultData)
{
    loger->info("TcpClient: ожидание ответа на запрос " + QString::number(requestId));
    if (requestId == -1) return false;
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(60000);
    connect(this, &TcpClient::responseReceived, &loop, [&](int id, bool, const QVariantMap&) {
        if (id == requestId) loop.quit();
    });
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    loop.exec();
    if (!timer.isActive()) {
        loger->error("TcpClient: таймаут ожидания ответа на запрос " + QString::number(requestId));
        throw NetworkEx("Таймаут ожидания ответа от сервера");
    }
    resultData = pendingResponses.take(requestId);
    bool success = resultData.value("_success").toBool();
    loger->info("TcpClient: получен ответ на запрос " + QString::number(requestId) + ", успех=" + QString::number(success));
    return success;
}

bool TcpClient::checkUser(const QString &login, const QString &password)
{
    loger->info("TcpClient::checkUser: логин " + login);
    try {
        QVariantMap data;
        data["login"] = login;
        data["password"] = password;
        int reqId = sendRequest("checkUser", data);
        QVariantMap res;
        if (waitForResponse(reqId, res)) {
            userRole = res["role"].toString();
            userName = res["name"].toString();
            userId = res["userId"].toInt();
            loger->info("TcpClient::checkUser: успешно, роль=" + userRole);
            return true;
        }
        return false;
    } catch (const NetworkEx &e) {
        loger->error(QString("checkUser: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("checkUser: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка связи с сервером при авторизации");
        return false;
    }
}

QString TcpClient::getUserName() const { return userName; }
QString TcpClient::getUserRole() const { return userRole; }
int TcpClient::getUserId() const { return userId; }

QVariantList TcpClient::getFoods()
{
    loger->info("TcpClient::getFoods");
    try {
        int reqId = sendRequest("getFoods");
        QVariantMap res;
        if (waitForResponse(reqId, res)) {
            QVariantList list = res["foods"].toList();
            loger->info("TcpClient::getFoods: получено " + QString::number(list.size()) + " блюд");
            return list;
        }
        return QVariantList();
    } catch (const NetworkEx &e) {
        loger->error(QString("getFoods: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getFoods: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка получения меню с сервера");
        return QVariantList();
    }
}

QVariantList TcpClient::getProducts()
{
    loger->info("TcpClient::getProducts");
    try {
        int reqId = sendRequest("getProducts");
        QVariantMap res;
        if (waitForResponse(reqId, res)) {
            QVariantList list = res["products"].toList();
            loger->info("TcpClient::getProducts: получено " + QString::number(list.size()) + " продуктов");
            return list;
        }
        return QVariantList();
    } catch (const NetworkEx &e) {
        loger->error(QString("getProducts: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getProducts: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка получения списка продуктов с сервера");
        return QVariantList();
    }
}

bool TcpClient::updateProductQuantity(int productId, int newQuantity)
{
    loger->info("TcpClient::updateProductQuantity: id=" + QString::number(productId) + ", new=" + QString::number(newQuantity));
    try {
        QVariantMap data;
        data["productId"] = productId;
        data["newQuantity"] = newQuantity;
        int reqId = sendRequest("updateProductQuantity", data);
        QVariantMap res;
        return waitForResponse(reqId, res);
    } catch (const NetworkEx &e) {
        loger->error(QString("updateProductQuantity: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("updateProductQuantity: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка обновления количества продукта");
        return false;
    }
}

bool TcpClient::addProductQuantity(int productId, int delta)
{
    loger->info("TcpClient::addProductQuantity: id=" + QString::number(productId) + ", delta=" + QString::number(delta));
    try {
        QVariantMap data;
        data["productId"] = productId;
        data["delta"] = delta;
        int reqId = sendRequest("addProductQuantity", data);
        QVariantMap res;
        return waitForResponse(reqId, res);
    } catch (const NetworkEx &e) {
        loger->error(QString("addProductQuantity: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("addProductQuantity: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка изменения количества продукта");
        return false;
    }
}

int TcpClient::createOrder(int tableNumber, int userId, const QString &specialRequests, double totalCost)
{
    loger->info("TcpClient::createOrder: table=" + QString::number(tableNumber) + ", user=" + QString::number(userId) + ", total=" + QString::number(totalCost));
    try {
        QVariantMap data;
        data["tableNumber"] = tableNumber;
        data["userId"] = userId;
        data["specialRequests"] = specialRequests;
        data["totalCost"] = totalCost;
        int reqId = sendRequest("createOrder", data);
        QVariantMap res;
        if (waitForResponse(reqId, res)) {
            int orderId = res["orderId"].toInt();
            loger->info("TcpClient::createOrder: заказ создан, id=" + QString::number(orderId));
            return orderId;
        }
        return -1;
    } catch (const NetworkEx &e) {
        loger->error(QString("createOrder: ") + e.what());
        emit errorOccurred(e.what());
        return -1;
    } catch (const std::exception &e) {
        loger->error(QString("createOrder: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка создания заказа на сервере");
        return -1;
    }
}

bool TcpClient::addOrderItem(int orderId, int foodId, int quantity, double price)
{
    loger->info("TcpClient::addOrderItem: order=" + QString::number(orderId) + ", food=" + QString::number(foodId));
    try {
        QVariantMap data;
        data["orderId"] = orderId;
        data["foodId"] = foodId;
        data["quantity"] = quantity;
        data["price"] = price;
        int reqId = sendRequest("addOrderItem", data);
        QVariantMap res;
        return waitForResponse(reqId, res);
    } catch (const NetworkEx &e) {
        loger->error(QString("addOrderItem: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("addOrderItem: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка добавления позиции заказа на сервере");
        return false;
    }
}

bool TcpClient::checkAndConsume(int foodId, int quantity)
{
    loger->info("TcpClient::checkAndConsume: food=" + QString::number(foodId) + ", qty=" + QString::number(quantity));
    try {
        QVariantMap data;
        data["foodId"] = foodId;
        data["quantity"] = quantity;
        int reqId = sendRequest("checkAndConsume", data);
        QVariantMap res;
        return waitForResponse(reqId, res);
    } catch (const NetworkEx &e) {
        loger->error(QString("checkAndConsume: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("checkAndConsume: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка при проверке и списании продуктов");
        return false;
    }
}

bool TcpClient::returnIngredients(int foodId, int quantity)
{
    loger->info("TcpClient::returnIngredients: food=" + QString::number(foodId) + ", qty=" + QString::number(quantity));
    try {
        QVariantMap data;
        data["foodId"] = foodId;
        data["quantity"] = quantity;
        int reqId = sendRequest("returnIngredients", data);
        QVariantMap res;
        return waitForResponse(reqId, res);
    } catch (const NetworkEx &e) {
        loger->error(QString("returnIngredients: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("returnIngredients: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка при возврате продуктов");
        return false;
    }
}

QVariantList TcpClient::getLowStockWarning(int foodId, int quantity)
{
    loger->info("TcpClient::getLowStockWarning: food=" + QString::number(foodId) + ", qty=" + QString::number(quantity));
    try {
        QVariantMap data;
        data["foodId"] = foodId;
        data["quantity"] = quantity;
        int reqId = sendRequest("getLowStockWarning", data);
        QVariantMap res;
        if (waitForResponse(reqId, res)) {
            return res["warnings"].toList();
        }
        return QVariantList();
    } catch (const NetworkEx &e) {
        loger->error(QString("getLowStockWarning: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getLowStockWarning: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка получения предупреждения о малых остатках");
        return QVariantList();
    }
}

QVariantMap TcpClient::getReportRevenue(const QString &from, const QString &to, int dishId, int tableNumber, int waiterId)
{
    loger->info("TcpClient::getReportRevenue: " + from + " - " + to);
    try {
        QVariantMap data;
        data["from"] = from;
        data["to"] = to;
        data["dishId"] = dishId;
        data["tableNumber"] = tableNumber;
        data["waiterId"] = waiterId;
        int reqId = sendRequest("getReportRevenue", data);
        QVariantMap res;
        if (waitForResponse(reqId, res)) {
            return res;
        }
        return QVariantMap();
    } catch (const NetworkEx &e) {
        loger->error(QString("getReportRevenue: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantMap();
    } catch (const std::exception &e) {
        loger->error(QString("getReportRevenue: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка получения отчёта о выручке");
        return QVariantMap();
    }
}

QVariantList TcpClient::getRevenueByDay(const QString &from, const QString &to, int dishId, int tableNumber, int waiterId)
{
    loger->info("TcpClient::getRevenueByDay: " + from + " - " + to);
    try {
        QVariantMap data;
        data["from"] = from;
        data["to"] = to;
        data["dishId"] = dishId;
        data["tableNumber"] = tableNumber;
        data["waiterId"] = waiterId;
        int reqId = sendRequest("getRevenueByDay", data);
        QVariantMap res;
        if (waitForResponse(reqId, res)) {
            return res["days"].toList();
        }
        return QVariantList();
    } catch (const NetworkEx &e) {
        loger->error(QString("getRevenueByDay: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getRevenueByDay: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка получения выручки по дням");
        return QVariantList();
    }
}

QVariantList TcpClient::getFoodPopularity(const QString &from, const QString &to)
{
    loger->info("TcpClient::getFoodPopularity: " + from + " - " + to);
    try {
        QVariantMap data;
        data["from"] = from;
        data["to"] = to;
        int reqId = sendRequest("getFoodPopularity", data);
        QVariantMap res;
        if (waitForResponse(reqId, res)) {
            return res["popularity"].toList();
        }
        return QVariantList();
    } catch (const NetworkEx &e) {
        loger->error(QString("getFoodPopularity: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getFoodPopularity: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка получения популярности блюд");
        return QVariantList();
    }
}

QVariantList TcpClient::getReportLowStock()
{
    loger->info("TcpClient::getReportLowStock");
    try {
        int reqId = sendRequest("getReportLowStock");
        QVariantMap res;
        if (waitForResponse(reqId, res)) {
            return res["lowStock"].toList();
        }
        return QVariantList();
    } catch (const NetworkEx &e) {
        loger->error(QString("getReportLowStock: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getReportLowStock: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка получения отчёта о малых остатках");
        return QVariantList();
    }
}

QVariantList TcpClient::getDishesList()
{
    loger->info("TcpClient::getDishesList");
    try {
        int reqId = sendRequest("getDishesList");
        QVariantMap res;
        if (waitForResponse(reqId, res)) {
            return res["dishes"].toList();
        }
        return QVariantList();
    } catch (const NetworkEx &e) {
        loger->error(QString("getDishesList: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getDishesList: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка получения списка блюд");
        return QVariantList();
    }
}

QVariantList TcpClient::getWaitersList()
{
    loger->info("TcpClient::getWaitersList");
    try {
        int reqId = sendRequest("getWaitersList");
        QVariantMap res;
        if (waitForResponse(reqId, res)) {
            return res["waiters"].toList();
        }
        return QVariantList();
    } catch (const NetworkEx &e) {
        loger->error(QString("getWaitersList: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getWaitersList: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка получения списка официантов");
        return QVariantList();
    }
}

QStringList TcpClient::getLocalIps()
{
    loger->info("TcpClient::getLocalIps");
    QStringList result;
    const QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : ifaces) {
        if (!(iface.flags() & QNetworkInterface::IsUp) || (iface.flags() & QNetworkInterface::IsLoopBack))
            continue;
        for (const QNetworkAddressEntry &entry : iface.addressEntries()) {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
                result << entry.ip().toString();
        }
    }
    loger->info("TcpClient::getLocalIps: найдено " + QString::number(result.size()) + " IP");
    return result;
}

bool TcpClient::pingServer(const QString &host, int port)
{
    loger->info("TcpClient::pingServer: проверка " + host + ":" + QString::number(port));
    QTcpSocket socket;
    socket.connectToHost(host, port);
    bool ok = socket.waitForConnected(2000);
    loger->info("TcpClient::pingServer: результат " + QString(ok ? "успех" : "неудача"));
    return ok;
}

bool TcpClient::reconnect(const QString &host, quint16 port)
{
    disconnectFromServer();
    return connectToServer(host, port);
}

bool TcpClient::saveReportToCSV(const QString &reportType,
                                const QString &from,
                                const QString &to,
                                int dishId,
                                int tableNumber,
                                int waiterId,
                                const QString &fileName)
{
    loger->info("TcpClient::saveReportToCSV: тип=" + reportType);
    try {
        QVariantMap data;
        data["reportType"] = reportType;
        data["from"] = from;
        data["to"] = to;
        data["dishId"] = dishId;
        data["tableNumber"] = tableNumber;
        data["waiterId"] = waiterId;
        data["fileName"] = fileName;   // передаём полный путь
        int reqId = sendRequest("saveReportToCSV", data);
        QVariantMap res;
        return waitForResponse(reqId, res);
    }
    catch (const NetworkEx &e) {
        loger->error("saveReportToCSV: " + QString(e.what()));
        emit errorOccurred(e.what());
        return false;
    }
}

QVariantList TcpClient::getOrders(const QString &status)
{
    loger->info("TcpClient::getOrders, status=" + status);
    try {
        QVariantMap data;
        if (!status.isEmpty())
            data["status"] = status;
        int reqId = sendRequest("getOrders", data);
        QVariantMap res;
        if (waitForResponse(reqId, res)) {
            return res["orders"].toList();
        }
        return QVariantList();
    } catch (const NetworkEx &e) {
        loger->error(QString("getOrders: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    }
}

bool TcpClient::updateOrderStatus(int orderId, const QString &newStatus)
{
    loger->info("TcpClient::updateOrderStatus: " + QString::number(orderId) + " -> " + newStatus);
    try {
        QVariantMap data;
        data["orderId"] = orderId;
        data["newStatus"] = newStatus;
        int reqId = sendRequest("updateOrderStatus", data);
        QVariantMap res;
        bool ok = waitForResponse(reqId, res);
        if (ok)
            emit ordersChanged(); // уведомить QML об изменении
        return ok;
    } catch (const NetworkEx &e) {
        loger->error(QString("updateOrderStatus: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    }
}

QVariantList TcpClient::getOrderItems(int orderId)
{
    loger->info("TcpClient::getOrderItems, orderId=" + QString::number(orderId));
    try {
        QVariantMap data;
        data["orderId"] = orderId;
        int reqId = sendRequest("getOrderItems", data);
        QVariantMap res;
        if (waitForResponse(reqId, res)) {
            return res["items"].toList();
        }
        return QVariantList();
    } catch (const NetworkEx &e) {
        loger->error(QString("getOrderItems: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    }
}

bool TcpClient::closeOrder(int orderId, double totalSum, const QVariantList &items)
{
    loger->info("TcpClient::closeOrder, orderId=" + QString::number(orderId));
    try {
        QVariantMap data;
        data["orderId"] = orderId;
        data["totalSum"] = totalSum;
        data["items"] = items;
        int reqId = sendRequest("closeOrder", data);
        QVariantMap res;
        bool ok = waitForResponse(reqId, res);
        if (ok)
            emit ordersChanged();
        return ok;
    } catch (const NetworkEx &e) {
        loger->error(QString("closeOrder: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    }
}

QVariantMap TcpClient::getCheckByOrderId(int orderId)
{
    loger->info("TcpClient::getCheckByOrderId, orderId=" + QString::number(orderId));
    try {
        QVariantMap data;
        data["orderId"] = orderId;
        int reqId = sendRequest("getCheckByOrderId", data);
        QVariantMap res;
        if (waitForResponse(reqId, res)) {
            return res;
        }
        return QVariantMap();
    } catch (const NetworkEx &e) {
        loger->error(QString("getCheckByOrderId: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantMap();
    }
}

bool TcpClient::syncOrderItems(int orderId, const QVariantList &newItems)
{
    loger->info("TcpClient::syncOrderItems, orderId=" + QString::number(orderId));
    try {
        QVariantMap data;
        data["orderId"] = orderId;
        data["newItems"] = newItems;
        int reqId = sendRequest("syncOrderItems", data);
        QVariantMap res;
        bool ok = waitForResponse(reqId, res);
        if (ok)
            emit ordersChanged();
        return ok;
    } catch (const NetworkEx &e) {
        loger->error(QString("syncOrderItems: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    }
}

QVariantList TcpClient::getClosedOrders()
{
    return getOrders("Закрыт");
}
