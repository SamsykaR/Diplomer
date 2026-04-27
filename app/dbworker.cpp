#include "dbworker.h"
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QSqlRecord>
#include <qdatetime.h>
#include <QNetworkInterface>
#include <QAbstractSocket>

DBWorker::DBWorker(Logs *logger, QObject *parent) : QObject(parent), loger(logger)
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("127.0.0.1");
    db.setPort(5432);
    db.setDatabaseName("RestN");
    db.setUserName("R");
    db.setPassword("");
    db.setConnectOptions("client_encoding=UTF8");

    if (!db.open()) {
        QString err = db.lastError().text();
        loger->warning("Ошибка подключения к БД: " + err);
        emit errorOccurred("Не удалось подключиться к базе данных: " + err);
    } else {
        loger->info("База данных подключена успешно!");
    }

    userId = -1;
    userName = "Гость";
    userRole = "Гость";
}

DBWorker::~DBWorker()
{
    if (db.isOpen())
        db.close();
}

bool DBWorker::checkUser(const QString &login, const QString &password)
{
    loger->info("Проверка пользователя: "+login);
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QSqlQuery query;
        query.prepare("SELECT UserId, FullName, Role, Salt, PasswordHash FROM Users WHERE Login = :login");
        query.bindValue(":login", login);

        if (!query.exec()) {
            throw DatabaseEx("Ошибка запроса checkUser: " + query.lastError().text());
        }

        if (query.next()) {
            QString salt = query.value("Salt").toString();
            QByteArray storedHash = query.value("PasswordHash").toByteArray();

            QString salted = password + salt;
            QByteArray computedHash = QCryptographicHash::hash(salted.toUtf8(), QCryptographicHash::Sha3_256);

            if (computedHash == storedHash) {
                userId = query.value("UserId").toInt();
                userName = query.value("FullName").toString();
                userRole = query.value("Role").toString();
                loger->info("Пользователь найден: " + userName);
                return true;
            }
        }
        loger->warning("Ошибка авторизации");
        return false;
    } catch (const DatabaseEx &e) {
        loger->error(QString("checkUser: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("checkUser: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при авторизации");
        return false;
    }
}

QVariantList DBWorker::getFoods()
{
    loger->info("Получение списка блюд");
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QVariantList result;
        QSqlQuery query;
        query.prepare(
            "SELECT f.FoodId, f.FullName, f.Description, f.Price, f.Category, f.Available, "
            "   CASE "
            "       WHEN f.Available = false THEN false "
            "       ELSE (SELECT bool_and(p.Quantity >= fc.Quantity) "
            "             FROM FoodComposition fc "
            "             JOIN Product p ON fc.ProductId = p.ProductId "
            "             WHERE fc.FoodId = f.FoodId) "
            "   END AS available_by_stock "
            "FROM Food f "
            "ORDER BY f.FullName"
            );
        if (!query.exec())
            throw DatabaseEx("Ошибка getFoods: " + query.lastError().text());

        while (query.next()) {
            QVariantMap item;
            item["foodId"] = query.value("FoodId").toInt();
            item["name"] = query.value("FullName").toString();
            item["description"] = query.value("Description").toString();
            item["price"] = query.value("Price").toDouble();
            item["category"] = query.value("Category").toString();
            item["available"] = query.value("available_by_stock").toBool();

            QSqlQuery compQuery;
            compQuery.prepare("SELECT p.FullName, fc.Quantity FROM FoodComposition fc "
                              "JOIN Product p ON fc.ProductId = p.ProductId "
                              "WHERE fc.FoodId = :foodId");
            compQuery.bindValue(":foodId", query.value("FoodId").toInt());
            QStringList composition;
            if (compQuery.exec()) {
                while (compQuery.next()) {
                    composition << QString("%1 (%2)").arg(compQuery.value(0).toString())
                    .arg(compQuery.value(1).toString());
                }
            }
            item["composition"] = composition.join(", ");
            result.append(item);
        }
        return result;
    } catch (const DatabaseEx &e) {
        loger->error(QString("getFoods: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getFoods: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при загрузке меню");
        return QVariantList();
    }
}

QVariantList DBWorker::getProducts()
{
    loger->info("Получение списка продуктов");
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QVariantList result;
        QSqlQuery query;
        query.prepare("SELECT ProductId, FullName, Unit, Quantity, MinThreshold FROM Product ORDER BY FullName");
        if (!query.exec())
            throw DatabaseEx("Ошибка получения продуктов: " + query.lastError().text());

        while (query.next()) {
            QVariantMap item;
            item["productId"] = query.value("ProductId").toInt();
            item["name"] = query.value("FullName").toString();
            item["unit"] = query.value("Unit").toString();
            item["quantity"] = query.value("Quantity").toInt();
            item["minThreshold"] = query.value("MinThreshold").toInt();
            result.append(item);
        }
        return result;
    } catch (const DatabaseEx &e) {
        loger->error(QString("getProducts: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getProducts: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при загрузке склада");
        return QVariantList();
    }
}

bool DBWorker::updateProductQuantity(int productId, int newQuantity)
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QSqlQuery query;
        query.prepare("UPDATE Product SET Quantity = :q WHERE ProductId = :id");
        query.bindValue(":id", productId);
        query.bindValue(":q", newQuantity);
        if (!query.exec())
            throw DatabaseEx("Ошибка обновления количества продуктов: " + query.lastError().text());
        return true;
    } catch (const DatabaseEx &e) {
        loger->error(QString("updateProductQuantity: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("updateProductQuantity: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при обновлении остатка");
        return false;
    }
}

bool DBWorker::addProductQuantity(int productId, int delta)
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QSqlQuery query;
        query.prepare("UPDATE Product SET Quantity = Quantity + :delta WHERE ProductId = :id");
        query.bindValue(":id", productId);
        query.bindValue(":delta", delta);
        if (!query.exec())
            throw DatabaseEx("Ошибка addProductQuantity: " + query.lastError().text());
        return true;
    } catch (const DatabaseEx &e) {
        loger->error(QString("addProductQuantity: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("addProductQuantity: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при изменении остатка");
        return false;
    }
}

int DBWorker::createOrder(int tableNumber, int userId, const QString &specialRequests, double totalCost)
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QSqlQuery query;
        query.prepare("INSERT INTO Orders (UserId, TableNumber, SpecialRequests, TotalCost) "
                      "VALUES (:uid, :table, :req, :total) RETURNING OrderId");
        query.bindValue(":uid", userId);
        query.bindValue(":table", tableNumber);
        query.bindValue(":req", specialRequests);
        query.bindValue(":total", totalCost);
        if (!query.exec())
            throw DatabaseEx("Ошибка createOrder: " + query.lastError().text());

        if (query.next())
            return query.value(0).toInt();
        else
            throw DatabaseEx("Не удалось получить OrderId после вставки");
    } catch (const DatabaseEx &e) {
        loger->error(QString("createOrder: ") + e.what());
        emit errorOccurred(e.what());
        return -1;
    } catch (const std::exception &e) {
        loger->error(QString("createOrder: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при создании заказа");
        return -1;
    }
}

bool DBWorker::addOrderItem(int orderId, int foodId, int quantity, double price)
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QSqlQuery query;
        query.prepare("INSERT INTO OrderFood (OrderId, FoodId, Quantity, Price) "
                      "VALUES (:oid, :fid, :qty, :price)");
        query.bindValue(":oid", orderId);
        query.bindValue(":fid", foodId);
        query.bindValue(":qty", quantity);
        query.bindValue(":price", price);
        if (!query.exec())
            throw DatabaseEx("Ошибка addOrderItem: " + query.lastError().text());
        return true;
    } catch (const DatabaseEx &e) {
        loger->error(QString("addOrderItem: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("addOrderItem: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при добавлении позиции заказа");
        return false;
    }
}

QVariantList DBWorker::getReportLowStock()
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QVariantList result;
        QSqlQuery query;
        query.prepare("SELECT FullName, Quantity, MinThreshold FROM Product WHERE Quantity < MinThreshold");
        if (!query.exec())
            throw DatabaseEx("Ошибка getReportLowStock: " + query.lastError().text());

        while (query.next()) {
            QVariantMap item;
            item["name"] = query.value(0).toString();
            item["quantity"] = query.value(1).toInt();
            item["minThreshold"] = query.value(2).toInt();
            result.append(item);
        }
        return result;
    } catch (const DatabaseEx &e) {
        loger->error(QString("getReportLowStock: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getReportLowStock: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при формировании отчёта");
        return QVariantList();
    }
}

QVariantList DBWorker::getFoodPopularity(const QString &from, const QString &to)
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QVariantList result;
        QDate fromDate = QDate::fromString(from, "yyyy-MM-dd");
        QDate toDate = QDate::fromString(to, "yyyy-MM-dd");
        if (!fromDate.isValid() || !toDate.isValid())
            throw ValidateEx("Неверный формат даты. Используйте ГГГГ-ММ-ДД");

        if (db.isOpen()) db.rollback();

        QString sql = QString(
                          "SELECT f.FullName, COALESCE(SUM(of.Quantity), 0) as total "
                          "FROM OrderFood of "
                          "JOIN Orders o ON of.OrderId = o.OrderId "
                          "JOIN Food f ON of.FoodId = f.FoodId "
                          "WHERE o.OrderDate >= '%1' AND o.OrderDate <= '%2' "
                          "GROUP BY f.FullName "
                          "ORDER BY total DESC"
                          ).arg(fromDate.toString("yyyy-MM-dd")).arg(toDate.toString("yyyy-MM-dd"));

        QSqlQuery query;
        if (!query.exec(sql))
            throw DatabaseEx("Ошибка getFoodPopularity: " + query.lastError().text());

        while (query.next()) {
            QVariantMap item;
            item["foodName"] = query.value(0).toString();
            item["count"] = query.value(1).toInt();
            result.append(item);
        }
        return result;
    } catch (const DatabaseEx &e) {
        loger->error(QString("getFoodPopularity: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const ValidateEx &e) {
        loger->warning(QString("getFoodPopularity: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getFoodPopularity: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при формировании отчёта популярности");
        return QVariantList();
    }
}

bool DBWorker::checkAndConsume(int foodId, int quantity)
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QSqlQuery compQuery;
        compQuery.prepare("SELECT ProductId, Quantity FROM FoodComposition WHERE FoodId = :foodId");
        compQuery.bindValue(":foodId", foodId);
        if (!compQuery.exec())
            throw DatabaseEx("Ошибка получения состава блюда: " + compQuery.lastError().text());

        QList<QPair<int, double>> composition;
        while (compQuery.next()) {
            int productId = compQuery.value(0).toInt();
            double need = compQuery.value(1).toDouble() * quantity;
            composition.append(qMakePair(productId, need));
        }
        if (composition.isEmpty())
            throw ValidateEx("Блюдо не имеет состава");

        for (const auto &pair : composition) {
            int productId = pair.first;
            double need = pair.second;

            QSqlQuery qtyQuery;
            qtyQuery.prepare("SELECT Quantity FROM Product WHERE ProductId = :pid");
            qtyQuery.bindValue(":pid", productId);
            if (!qtyQuery.exec() || !qtyQuery.next())
                throw DatabaseEx(QString("Ошибка получения остатка продукта %1").arg(productId));

            double current = qtyQuery.value(0).toDouble();
            if (current < need - 0.0001) {
                throw ValidateEx(QString("Не хватает продукта id=%1, нужно %2, есть %3")
                                     .arg(productId).arg(need).arg(current));
            }
        }

        QSqlDatabase db = QSqlDatabase::database();
        if (db.isOpen() && db.isValid())
            db.rollback();

        if (!db.transaction())
            throw DatabaseEx("Не удалось начать транзакцию");

        bool ok = true;
        for (const auto &pair : composition) {
            int productId = pair.first;
            double need = pair.second;
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE Product SET Quantity = Quantity - :need WHERE ProductId = :pid AND Quantity >= :need");
            updateQuery.bindValue(":need", need);
            updateQuery.bindValue(":pid", productId);
            if (!updateQuery.exec()) {
                db.rollback();
                throw DatabaseEx(QString("Ошибка UPDATE при списании: %1").arg(updateQuery.lastError().text()));
            }
            if (updateQuery.numRowsAffected() == 0) {
                db.rollback();
                throw ValidateEx(QString("Недостаточно товара для списания (параллельное изменение?) productId=%1").arg(productId));
            }
        }
        if (!db.commit()) {
            db.rollback();
            throw DatabaseEx("Ошибка коммита транзакции");
        }
        return true;
    } catch (const DatabaseEx &e) {
        loger->error(QString("checkAndConsume: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const ValidateEx &e) {
        loger->warning(QString("checkAndConsume: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("checkAndConsume: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при добавлении блюда");
        return false;
    }
}

bool DBWorker::returnIngredients(int foodId, int quantity)
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QSqlQuery compQuery;
        compQuery.prepare("SELECT ProductId, Quantity FROM FoodComposition WHERE FoodId = :foodId");
        compQuery.bindValue(":foodId", foodId);
        if (!compQuery.exec())
            throw DatabaseEx("Ошибка получения состава для возврата: " + compQuery.lastError().text());

        QList<QPair<int, double>> composition;
        while (compQuery.next()) {
            int productId = compQuery.value(0).toInt();
            double amount = compQuery.value(1).toDouble() * quantity;
            composition.append(qMakePair(productId, amount));
        }
        if (composition.isEmpty())
            throw ValidateEx("Блюдо не имеет состава, возврат невозможен");

        QSqlDatabase db = QSqlDatabase::database();
        if (db.isOpen() && db.isValid())
            db.rollback();

        if (!db.transaction())
            throw DatabaseEx("Не удалось начать транзакцию для возврата");

        for (const auto &pair : composition) {
            int productId = pair.first;
            double amount = pair.second;
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE Product SET Quantity = Quantity + :amount WHERE ProductId = :pid");
            updateQuery.bindValue(":amount", amount);
            updateQuery.bindValue(":pid", productId);
            if (!updateQuery.exec()) {
                db.rollback();
                throw DatabaseEx(QString("Ошибка UPDATE при возврате: %1").arg(updateQuery.lastError().text()));
            }
        }
        if (!db.commit()) {
            db.rollback();
            throw DatabaseEx("Ошибка коммита при возврате");
        }
        return true;
    } catch (const DatabaseEx &e) {
        loger->error(QString("returnIngredients: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const ValidateEx &e) {
        loger->warning(QString("returnIngredients: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("returnIngredients: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при возврате продуктов");
        return false;
    }
}

bool DBWorker::refreshAllFoodsAvailability()
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QSqlQuery foodsQuery;
        foodsQuery.prepare("SELECT FoodId FROM Food");
        if (!foodsQuery.exec())
            throw DatabaseEx("Ошибка получения списка блюд: " + foodsQuery.lastError().text());

        QSqlDatabase::database().transaction();
        bool allOk = true;
        while (foodsQuery.next()) {
            int foodId = foodsQuery.value(0).toInt();

            QSqlQuery compQuery;
            compQuery.prepare("SELECT ProductId, Quantity FROM FoodComposition WHERE FoodId = :foodId");
            compQuery.bindValue(":foodId", foodId);
            if (!compQuery.exec())
                throw DatabaseEx("Ошибка состава блюда: " + compQuery.lastError().text());

            bool available = true;
            while (compQuery.next()) {
                int productId = compQuery.value(0).toInt();
                double needed = compQuery.value(1).toDouble();

                QSqlQuery stockQuery;
                stockQuery.prepare("SELECT Quantity FROM Product WHERE ProductId = :pid");
                stockQuery.bindValue(":pid", productId);
                if (!stockQuery.exec() || !stockQuery.next())
                    throw DatabaseEx("Ошибка остатка продукта: " + stockQuery.lastError().text());

                double current = stockQuery.value(0).toDouble();
                if (current < needed - 0.0001) {
                    available = false;
                    break;
                }
            }

            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE Food SET Available = :avail WHERE FoodId = :foodId");
            updateQuery.bindValue(":avail", available);
            updateQuery.bindValue(":foodId", foodId);
            if (!updateQuery.exec())
                throw DatabaseEx("Ошибка обновления доступности блюда: " + updateQuery.lastError().text());
        }

        if (allOk) {
            QSqlDatabase::database().commit();
            emit foodAvailabilityChanged();
            return true;
        } else {
            QSqlDatabase::database().rollback();
            return false;
        }
    } catch (const DatabaseEx &e) {
        loger->error(QString("refreshAllFoodsAvailability: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("refreshAllFoodsAvailability: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при обновлении доступности");
        return false;
    }
}

QVariantList DBWorker::getLowStockWarning(int foodId, int quantity)
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QVariantList result;
        QSqlQuery compQuery;
        compQuery.prepare("SELECT ProductId, Quantity FROM FoodComposition WHERE FoodId = :foodId");
        compQuery.bindValue(":foodId", foodId);
        if (!compQuery.exec())
            throw DatabaseEx("Ошибка получения состава для предупреждения: " + compQuery.lastError().text());

        while (compQuery.next()) {
            int productId = compQuery.value(0).toInt();
            double need = compQuery.value(1).toDouble() * quantity;

            QSqlQuery prodQuery;
            prodQuery.prepare("SELECT FullName, Quantity, MinThreshold FROM Product WHERE ProductId = :pid");
            prodQuery.bindValue(":pid", productId);
            if (!prodQuery.exec() || !prodQuery.next())
                continue;

            QString name = prodQuery.value(0).toString();
            double current = prodQuery.value(1).toDouble();
            double minThr = prodQuery.value(2).toDouble();
            double newQty = current - need;

            if (newQty < minThr) {
                QVariantMap warning;
                warning["productName"] = name;
                warning["currentQuantity"] = current;
                warning["newQuantity"] = newQty;
                warning["minThreshold"] = minThr;
                result.append(warning);
            }
        }
        return result;
    } catch (const DatabaseEx &e) {
        loger->error(QString("getLowStockWarning: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getLowStockWarning: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при проверке остатков");
        return QVariantList();
    }
}

QVariantList DBWorker::getDishesList()
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QVariantList result;
        QSqlQuery query;
        query.prepare("SELECT FoodId, FullName FROM Food ORDER BY FullName");
        if (!query.exec())
            throw DatabaseEx("Ошибка getDishesList: " + query.lastError().text());

        while (query.next()) {
            QVariantMap item;
            item["id"] = query.value(0).toInt();
            item["name"] = query.value(1).toString();
            result.append(item);
        }
        return result;
    } catch (const DatabaseEx &e) {
        loger->error(QString("getDishesList: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getDishesList: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при загрузке списка блюд");
        return QVariantList();
    }
}

QVariantList DBWorker::getWaitersList()
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QVariantList result;
        QSqlQuery query;
        query.prepare("SELECT UserId, FullName FROM Users WHERE Role = 'waiter' ORDER BY FullName");
        if (!query.exec())
            throw DatabaseEx("Ошибка getWaitersList: " + query.lastError().text());

        while (query.next()) {
            QVariantMap item;
            item["id"] = query.value(0).toInt();
            item["name"] = query.value(1).toString();
            result.append(item);
        }
        return result;
    } catch (const DatabaseEx &e) {
        loger->error(QString("getWaitersList: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getWaitersList: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при загрузке списка официантов");
        return QVariantList();
    }
}

QVariantMap DBWorker::getReportRevenue(const QString &from, const QString &to,
                                       int dishId, int tableNumber, int waiterId)
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QVariantMap result;
        QDate fromDate = QDate::fromString(from, "yyyy-MM-dd");
        QDate toDate = QDate::fromString(to, "yyyy-MM-dd");
        if (!fromDate.isValid() || !toDate.isValid()) {
            result["error"] = "Неверный формат даты. Используйте ГГГГ-ММ-ДД";
            return result;
        }

        if (db.isOpen()) db.rollback();

        QString sql = "SELECT COALESCE(SUM(o.TotalCost), 0) FROM Orders o ";
        QString where = "WHERE o.OrderDate >= :from AND o.OrderDate <= :to";

        if (dishId > 0) {
            sql += "JOIN OrderFood of ON o.OrderId = of.OrderId ";
            where += " AND of.FoodId = :dishId";
        }
        if (tableNumber > 0) {
            where += " AND o.TableNumber = :table";
        }
        if (waiterId > 0) {
            where += " AND o.UserId = :waiter";
        }
        sql += where;

        QSqlQuery query;
        query.prepare(sql);
        query.bindValue(":from", fromDate);
        query.bindValue(":to", toDate);
        if (dishId > 0) query.bindValue(":dishId", dishId);
        if (tableNumber > 0) query.bindValue(":table", tableNumber);
        if (waiterId > 0) query.bindValue(":waiter", waiterId);

        if (!query.exec())
            throw DatabaseEx("Ошибка getReportRevenue: " + query.lastError().text());

        if (query.next()) {
            result["totalRevenue"] = query.value(0).toDouble();
        } else {
            result["totalRevenue"] = 0.0;
        }
        return result;
    } catch (const DatabaseEx &e) {
        loger->error(QString("getReportRevenue: ") + e.what());
        emit errorOccurred(e.what());
        QVariantMap err;
        err["error"] = e.what();
        return err;
    } catch (const std::exception &e) {
        loger->error(QString("getReportRevenue: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при формировании отчёта");
        QVariantMap err;
        err["error"] = "Внутренняя ошибка";
        return err;
    }
}

QVariantList DBWorker::getRevenueByDay(const QString &from, const QString &to,
                                       int dishId, int tableNumber, int waiterId)
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QVariantList result;
        QDate fromDate = QDate::fromString(from, "yyyy-MM-dd");
        QDate toDate = QDate::fromString(to, "yyyy-MM-dd");
        if (!fromDate.isValid() || !toDate.isValid())
            throw ValidateEx("Неверный формат даты");

        if (db.isOpen()) db.rollback();

        QString sql = "SELECT DATE(o.OrderDate) as day, COALESCE(SUM(o.TotalCost), 0) as total "
                      "FROM Orders o ";
        QString where = "WHERE o.OrderDate >= :from AND o.OrderDate <= :to";

        if (dishId > 0) {
            sql += "JOIN OrderFood of ON o.OrderId = of.OrderId ";
            where += " AND of.FoodId = :dishId";
        }
        if (tableNumber > 0) {
            where += " AND o.TableNumber = :table";
        }
        if (waiterId > 0) {
            where += " AND o.UserId = :waiter";
        }
        sql += where + " GROUP BY DATE(o.OrderDate) ORDER BY day";

        QSqlQuery query;
        query.prepare(sql);
        query.bindValue(":from", fromDate);
        query.bindValue(":to", toDate);
        if (dishId > 0) query.bindValue(":dishId", dishId);
        if (tableNumber > 0) query.bindValue(":table", tableNumber);
        if (waiterId > 0) query.bindValue(":waiter", waiterId);

        if (!query.exec())
            throw DatabaseEx("Ошибка getRevenueByDay: " + query.lastError().text());

        while (query.next()) {
            QVariantMap dayReport;
            dayReport["date"] = query.value(0).toDate().toString("yyyy-MM-dd");
            dayReport["total"] = query.value(1).toDouble();
            result.append(dayReport);
        }
        return result;
    } catch (const DatabaseEx &e) {
        loger->error(QString("getRevenueByDay: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const ValidateEx &e) {
        loger->warning(QString("getRevenueByDay: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getRevenueByDay: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при формировании отчёта по дням");
        return QVariantList();
    }
}

bool DBWorker::reconnect(const QString &host, int port, const QString &dbName, const QString &user, const QString &password)
{
    try {
        if (db.isOpen()) db.close();
        db.setHostName(host);
        db.setPort(port);
        db.setDatabaseName(dbName);
        db.setUserName(user);
        db.setPassword(password);
        if (!db.open())
            throw DatabaseEx("Ошибка подключения к БД: " + db.lastError().text());
        return true;
    } catch (const DatabaseEx &e) {
        loger->error(QString("reconnect: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("reconnect: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Не удалось переподключиться к базе данных");
        return false;
    }
}

QStringList DBWorker::getLocalIps()
{
    QStringList result;
    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : interfaces) {
        if (!(iface.flags() & QNetworkInterface::IsUp) || (iface.flags() & QNetworkInterface::IsLoopBack))
            continue;
        for (const QNetworkAddressEntry &entry : iface.addressEntries()) {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                result << entry.ip().toString();
            }
        }
    }
    return result;
}

bool DBWorker::pingServer(const QString &host, int port)
{
    Q_UNUSED(host);
    Q_UNUSED(port);
    return true;
}
