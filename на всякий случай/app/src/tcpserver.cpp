#include "tcpserver.h"
#include "dbworker.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

ClientConnection::ClientConnection(Logs *logger, qintptr socketDescriptor, DBWorker *dbWorker, QObject *parent)
    : QObject(parent), dbWorker(dbWorker), loger(logger)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &ClientConnection::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ClientConnection::onDisconnected);
}

ClientConnection::~ClientConnection()
{
    socket->deleteLater();
}

void ClientConnection::sendResponse(int requestId, bool success, const QVariantMap &data)
{
    QJsonObject resp;
    resp["response"] = success;
    resp["requestId"] = requestId;
    if (!data.isEmpty())
        resp["data"] = QJsonObject::fromVariantMap(data);
    QByteArray msg = QJsonDocument(resp).toJson(QJsonDocument::Compact) + "\n";
    qint64 bytes = socket->write(msg);
    socket->flush();
    loger->info("ClientConnection: отправлен ответ на запрос " + QString::number(requestId) +
                ", успех=" + QString::number(success) + ", байт=" + QString::number(bytes));
}

void ClientConnection::onReadyRead()
{
    QByteArray data = socket->readAll();
    loger->info("ClientConnection: получено " + QString::number(data.size()) + " байт");
    buffer.append(data);
    while (true) {
        int nl = buffer.indexOf('\n');
        if (nl == -1) break;
        QByteArray line = buffer.left(nl);
        buffer.remove(0, nl + 1);
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(line, &err);
        if (err.error != QJsonParseError::NoError) continue;
        if (!doc.isObject()) continue;
        QJsonObject obj = doc.object();
        int requestId = obj.value("requestId").toInt();
        QString command = obj.value("command").toString();
        QVariantMap data = obj.value("data").toObject().toVariantMap();

        if (command == "checkUser") {
            try {
                bool ok = dbWorker->checkUser(data["login"].toString(), data["password"].toString());
                if (ok) {
                    QVariantMap result;
                    result["role"] = dbWorker->getUserRole();
                    result["name"] = dbWorker->getUserName();
                    result["userId"] = dbWorker->getUserId();
                    sendResponse(requestId, true, result);
                } else {
                    sendResponse(requestId, false);
                }
            } catch (const std::exception &e) {
                loger->error("Исключение в checkUser: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "getFoods") {
            try {
                QVariantList foods = dbWorker->getFoods();
                sendResponse(requestId, true, {{"foods", foods}});
            } catch (const std::exception &e) {
                loger->error("Исключение в getFoods: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "getProducts") {
            try {
                QVariantList products = dbWorker->getProducts();
                sendResponse(requestId, true, {{"products", products}});
            } catch (const std::exception &e) {
                loger->error("Исключение в getProducts: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "updateProductQuantity") {
            try {
                bool ok = dbWorker->updateProductQuantity(data["productId"].toInt(), data["newQuantity"].toInt());
                sendResponse(requestId, ok);
            } catch (const std::exception &e) {
                loger->error("Исключение в updateProductQuantity: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "addProductQuantity") {
            try {
                bool ok = dbWorker->addProductQuantity(data["productId"].toInt(), data["delta"].toInt());
                sendResponse(requestId, ok);
            } catch (const std::exception &e) {
                loger->error("Исключение в addProductQuantity: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "createOrder") {
            try {
                int orderId = dbWorker->createOrder(data["tableNumber"].toInt(), data["userId"].toInt(),
                                                    data["specialRequests"].toString(), data["totalCost"].toDouble());
                if (orderId > 0) {
                    sendResponse(requestId, true, {{"orderId", orderId}});
                } else {
                    sendResponse(requestId, false);
                }
            } catch (const std::exception &e) {
                loger->error("Исключение в createOrder: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "addOrderItem") {
            try {
                bool ok = dbWorker->addOrderItem(data["orderId"].toInt(), data["foodId"].toInt(),
                                                 data["quantity"].toInt(), data["price"].toDouble());
                sendResponse(requestId, ok);
            } catch (const std::exception &e) {
                loger->error("Исключение в addOrderItem: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "checkAndConsume") {
            try {
                bool ok = dbWorker->checkAndConsume(data["foodId"].toInt(), data["quantity"].toInt());
                sendResponse(requestId, ok);
            } catch (const std::exception &e) {
                loger->error("Исключение в checkAndConsume: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "returnIngredients") {
            try {
                bool ok = dbWorker->returnIngredients(data["foodId"].toInt(), data["quantity"].toInt());
                sendResponse(requestId, ok);
            } catch (const std::exception &e) {
                loger->error("Исключение в returnIngredients: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "getLowStockWarning") {
            try {
                QVariantList warnings = dbWorker->getLowStockWarning(data["foodId"].toInt(), data["quantity"].toInt());
                sendResponse(requestId, true, {{"warnings", warnings}});
            } catch (const std::exception &e) {
                loger->error("Исключение в getLowStockWarning: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "getReportRevenue") {
            try {
                QVariantMap rev = dbWorker->getReportRevenue(data["from"].toString(), data["to"].toString(),
                                                             data["dishId"].toInt(), data["tableNumber"].toInt(),
                                                             data["waiterId"].toInt());
                sendResponse(requestId, true, rev);
            } catch (const std::exception &e) {
                loger->error("Исключение в getReportRevenue: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "getRevenueByDay") {
            try {
                QVariantList days = dbWorker->getRevenueByDay(data["from"].toString(), data["to"].toString(),
                                                              data["dishId"].toInt(), data["tableNumber"].toInt(),
                                                              data["waiterId"].toInt());
                sendResponse(requestId, true, {{"days", days}});
            } catch (const std::exception &e) {
                loger->error("Исключение в getRevenueByDay: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "getFoodPopularity") {
            try {
                QVariantList pop = dbWorker->getFoodPopularity(data["from"].toString(), data["to"].toString());
                sendResponse(requestId, true, {{"popularity", pop}});
            } catch (const std::exception &e) {
                loger->error("Исключение в getFoodPopularity: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "getReportLowStock") {
            try {
                QVariantList low = dbWorker->getReportLowStock();
                sendResponse(requestId, true, {{"lowStock", low}});
            } catch (const std::exception &e) {
                loger->error("Исключение в getReportLowStock: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "getDishesList") {
            try {
                QVariantList dishes = dbWorker->getDishesList();
                sendResponse(requestId, true, {{"dishes", dishes}});
            } catch (const std::exception &e) {
                loger->error("Исключение в getDishesList: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "getWaitersList") {
            try {
                QVariantList waiters = dbWorker->getWaitersList();
                sendResponse(requestId, true, {{"waiters", waiters}});
            } catch (const std::exception &e) {
                loger->error("Исключение в getWaitersList: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
        else if (command == "saveReportToCSV") {
            try {
                bool ok = dbWorker->saveReportToCSV(
                    data["reportType"].toString(),
                    data["from"].toString(),
                    data["to"].toString(),
                    data["dishId"].toInt(),
                    data["tableNumber"].toInt(),
                    data["waiterId"].toInt(),
                    data["fileName"].toString()
                    );
                sendResponse(requestId, ok);
            } catch (const std::exception &e) {
                loger->error("Исключение в saveReportToCSV: " + QString(e.what()));
                sendResponse(requestId, false, {{"error", e.what()}});
            }
        }
		else if (command == "getOrders") {
    try {
        QString status = data["status"].toString();
        QVariantList orders = dbWorker->getOrders(status);
        sendResponse(requestId, true, {{"orders", orders}});
    } catch (const std::exception &e) {
        loger->error("Исключение в getOrders: " + QString(e.what()));
        sendResponse(requestId, false, {{"error", e.what()}});
    }
}
else if (command == "updateOrderStatus") {
    try {
        bool ok = dbWorker->updateOrderStatus(data["orderId"].toInt(), data["newStatus"].toString());
        sendResponse(requestId, ok);
    } catch (const std::exception &e) {
        loger->error("Исключение в updateOrderStatus: " + QString(e.what()));
        sendResponse(requestId, false, {{"error", e.what()}});
    }
}
else if (command == "getOrderItems") {
    try {
        QVariantList items = dbWorker->getOrderItems(data["orderId"].toInt());
        sendResponse(requestId, true, {{"items", items}});
    } catch (const std::exception &e) {
        loger->error("Исключение в getOrderItems: " + QString(e.what()));
        sendResponse(requestId, false, {{"error", e.what()}});
    }
}
else if (command == "closeOrder") {
    try {
        bool ok = dbWorker->closeOrder(data["orderId"].toInt(), data["totalSum"].toDouble(), data["items"].toList());
        sendResponse(requestId, ok);
    } catch (const std::exception &e) {
        loger->error("Исключение в closeOrder: " + QString(e.what()));
        sendResponse(requestId, false, {{"error", e.what()}});
    }
}
else if (command == "getCheckByOrderId") {
    try {
        QVariantMap check = dbWorker->getCheckByOrderId(data["orderId"].toInt());
        sendResponse(requestId, true, check);
    } catch (const std::exception &e) {
        loger->error("Исключение в getCheckByOrderId: " + QString(e.what()));
        sendResponse(requestId, false, {{"error", e.what()}});
    }
}
else if (command == "syncOrderItems") {
    try {
        bool ok = dbWorker->syncOrderItems(data["orderId"].toInt(), data["newItems"].toList());
        sendResponse(requestId, ok);
    } catch (const std::exception &e) {
        loger->error("Исключение в syncOrderItems: " + QString(e.what()));
        sendResponse(requestId, false, {{"error", e.what()}});
    }
}
        else {
            sendResponse(requestId, false, {{"error", "Unknown command"}});
        }
    }
}

void ClientConnection::onDisconnected()
{
    deleteLater();
}

TcpServer::TcpServer(Logs *logger, DBWorker *dbWorker, QObject *parent)
    : QObject(parent), server(nullptr), dbWorker(dbWorker), loger(logger)
{
    loger->info("Инициализация TCP-сервера");
}

TcpServer::~TcpServer()
{
    stop();
    loger->info("TCP-сервер уничтожен");
}

bool TcpServer::start(quint16 port)
{
    if (server) {
        loger->warning("Сервер уже запущен");
        return false;
    }
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &TcpServer::onNewConnection);
    if (!server->listen(QHostAddress::Any, port)) {
        loger->error("Ошибка запуска сервера: " + server->errorString());
        delete server;
        server = nullptr;
        return false;
    }
    loger->info("Сервер запущен на порту " + QString::number(port));
    return true;
}

void TcpServer::stop()
{
    if (server) {
        server->close();
        delete server;
        server = nullptr;
        loger->info("Сервер остановлен");
    }
    qDeleteAll(connections);
    connections.clear();
}

void TcpServer::onNewConnection()
{
    QTcpSocket *clientSocket = server->nextPendingConnection();
    ClientConnection *conn = new ClientConnection(loger, clientSocket->socketDescriptor(), dbWorker, this);
    connections.append(conn);
    loger->info("Новое подключение, всего клиентов: " + QString::number(connections.size()));
}
