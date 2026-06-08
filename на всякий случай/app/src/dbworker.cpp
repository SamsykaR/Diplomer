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
        if (!db.isOpen()) throw DatabaseEx("База данных не открыта");
        int orderNumber = generateOrderNumber();
        if (orderNumber == -1) throw DatabaseEx("Не удалось сгенерировать номер заказа");

        QSqlQuery query;
        query.prepare("INSERT INTO Orders (UserId, TableNumber, SpecialRequests, TotalCost, order_number, status) "
                      "VALUES (:uid, :table, :req, :total, :onum, 'Готовится') RETURNING OrderId");
        query.bindValue(":uid", userId);
        query.bindValue(":table", tableNumber);
        query.bindValue(":req", specialRequests);
        query.bindValue(":total", totalCost);
        query.bindValue(":onum", orderNumber);
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

bool DBWorker::addOrderItem(int OrderId, int foodId, int quantity, double price)
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QSqlQuery query;
        query.prepare("INSERT INTO OrderFood (OrderId, FoodId, Quantity, Price) "
                      "VALUES (:oid, :fid, :qty, :price)");
        query.bindValue(":oid", OrderId);
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

        if (!db.transaction())
            throw DatabaseEx("Не удалось начать транзакцию");

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

bool DBWorker::saveReportToCSV(const QString &reportType,
                               const QString &from,
                               const QString &to,
                               int dishId,
                               int tableNumber,
                               int waiterId,
                               const QString &fileName)
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            throw FileEx("Не удалось открыть файл для записи: " + fileName);
        }
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);

        if (reportType == "revenue") {
            // Отчёт по выручке
            QVariantMap total = getReportRevenue(from, to, dishId, tableNumber, waiterId);
            QVariantList days = getRevenueByDay(from, to, dishId, tableNumber, waiterId);

            out << "Общая выручка," << total["totalRevenue"].toDouble() << "\n";
            out << "\nДата,Выручка\n";
            for (const QVariant &day : days) {
                QVariantMap d = day.toMap();
                out << d["date"].toString() << "," << d["total"].toDouble() << "\n";
            }
        }
        else if (reportType == "popularity") {
            QVariantList pop = getFoodPopularity(from, to);
            out << "Блюдо,Количество порций\n";
            for (const QVariant &item : pop) {
                QVariantMap m = item.toMap();
                out << m["foodName"].toString() << "," << m["count"].toInt() << "\n";
            }
        }
        else if (reportType == "lowstock") {
            QVariantList low = getReportLowStock();
            out << "Продукт,Текущее количество,Минимальный порог\n";
            for (const QVariant &item : low) {
                QVariantMap m = item.toMap();
                out << m["name"].toString() << ","
                    << m["quantity"].toInt() << ","
                    << m["minThreshold"].toInt() << "\n";
            }
        }
        else {
            throw ValidateEx("Неизвестный тип отчёта");
        }

        file.close();
        loger->info("Отчёт сохранён в файл: " + fileName);
        return true;
    }
    catch (const DatabaseEx &e) {
        loger->error("saveReportToCSV: " + QString(e.what()));
        emit errorOccurred(e.what());
        return false;
    }
    catch (const FileEx &e) {
        loger->error("saveReportToCSV: " + QString(e.what()));
        emit errorOccurred(e.what());
        return false;
    }
    catch (const std::exception &e) {
        loger->error("saveReportToCSV: " + QString(e.what()));
        emit errorOccurred("Внутренняя ошибка при сохранении отчёта");
        return false;
    }
}

// ========== Генератор номера заказа (ежедневный сброс) ==========
int DBWorker::generateOrderNumber()
{
    try {
        if (!db.isOpen()) throw DatabaseEx("База данных не открыта");
        QSqlQuery query;
        // Получаем максимальный номер заказа за сегодня
        query.prepare("SELECT COALESCE(MAX(order_number), 0) + 1 FROM Orders WHERE DATE(OrderDate) = CURRENT_DATE");
        if (!query.exec())
            throw DatabaseEx("Ошибка при генерации номера заказа: " + query.lastError().text());
        if (query.next())
            return query.value(0).toInt();
        else
            return 1;
    } catch (const DatabaseEx &e) {
        loger->error(QString("generateOrderNumber: ") + e.what());
        emit errorOccurred(e.what());
        return -1;
    } catch (const std::exception &e) {
        loger->error(QString("generateOrderNumber: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при генерации номера заказа");
        return -1;
    }
}

// ========== Получение списка заказов ==========
QVariantList DBWorker::getOrders(const QString &status)
{
    try {
        if (!db.isOpen()) throw DatabaseEx("База данных не открыта");
        QVariantList result;
        QString sql = "SELECT o.OrderId, o.order_number, o.TableNumber, o.OrderDate, "
                      "o.SpecialRequests, o.TotalCost, o.status, u.FullName AS waiterName "
                      "FROM Orders o "
                      "LEFT JOIN Users u ON o.UserId = u.UserId ";
        if (!status.isEmpty())
            sql += "WHERE status = '" + status + "' ";
        sql += "ORDER BY o.OrderDate DESC";

        QSqlQuery query;
        if (!query.exec(sql))
            throw DatabaseEx("Ошибка getOrders: " + query.lastError().text());

        while (query.next()) {
            QVariantMap order;
            order["orderId"] = query.value("OrderId").toInt();                    // было OrderId -> orderId
            order["orderNumber"] = query.value("order_number").toInt();
            order["tableNumber"] = query.value("TableNumber").toInt();            // TableNumber -> tableNumber
            order["orderDate"] = query.value("OrderDate").toDateTime();
            order["specialRequests"] = query.value("SpecialRequests").toString();
            order["totalCost"] = query.value("TotalCost").toDouble();             // TotalCost -> totalCost
            order["status"] = query.value("status").toString();
            order["waiterName"] = query.value("waiterName").toString();   // новое поле
            result.append(order);
        }
        loger->info("getOrders returned " + QString::number(result.size()) + " orders");
        return result;
    } catch (const DatabaseEx &e) {
        loger->error(QString("getOrders: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getOrders: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при загрузке заказов");
        return QVariantList();
    }
}

// ========== Обновление статуса заказа ==========
bool DBWorker::updateOrderStatus(int OrderId, const QString &newStatus)
{
    try {
        if (!db.isOpen()) throw DatabaseEx("База данных не открыта");
        QSqlQuery query;
        query.prepare("UPDATE Orders SET status = :status WHERE OrderId = :id");
        query.bindValue(":status", newStatus);
        query.bindValue(":id", OrderId);
        if (!query.exec())
            throw DatabaseEx("Ошибка updateOrderStatus: " + query.lastError().text());
        loger->info("Статус заказа " + QString::number(OrderId) + " изменён на " + newStatus);
        emit ordersChanged();
        return true;
    } catch (const DatabaseEx &e) {
        loger->error(QString("updateOrderStatus: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("updateOrderStatus: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка при изменении статуса заказа");
        return false;
    }
}

// ========== Получение позиций заказа ==========
QVariantList DBWorker::getOrderItems(int OrderId)
{
    try {
        if (!db.isOpen()) throw DatabaseEx("База данных не открыта");
        QVariantList result;
        QSqlQuery query;
        query.prepare("SELECT of.OrderFoodId, of.FoodId, f.FullName, of.Quantity, of.Price "
                      "FROM OrderFood of "
                      "JOIN Food f ON of.FoodId = f.FoodId "
                      "WHERE of.OrderId = :OrderId");
        query.bindValue(":OrderId", OrderId);
        if (!query.exec())
            throw DatabaseEx("Ошибка getOrderItems: " + query.lastError().text());
        while (query.next()) {
            QVariantMap item;
            item["orderFoodId"] = query.value("OrderFoodId").toInt();
            item["foodId"] = query.value("FoodId").toInt();
            item["name"] = query.value("FullName").toString();
            item["quantity"] = query.value("Quantity").toInt();
            item["price"] = query.value("Price").toDouble();
            result.append(item);
        }
        return result;
    } catch (const DatabaseEx &e) {
        loger->error(QString("getOrderItems: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    } catch (const std::exception &e) {
        loger->error(QString("getOrderItems: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка загрузки позиций заказа");
        return QVariantList();
    }
}

// ========== Закрытие заказа и создание чека ==========
bool DBWorker::closeOrder(int OrderId, double totalSum, const QVariantList &items)
{
    try {
        if (!db.isOpen()) throw DatabaseEx("База данных не открыта");

        // 1. Получить номер заказа и дату
        QSqlQuery orderQuery;
        orderQuery.prepare("SELECT order_number, OrderDate FROM Orders WHERE OrderId = :id");
        orderQuery.bindValue(":id", OrderId);
        if (!orderQuery.exec() || !orderQuery.next())
            throw DatabaseEx("Не удалось получить данные заказа");
        int orderNumber = orderQuery.value("order_number").toInt();
        QDate orderDate = orderQuery.value("OrderDate").toDate();

        // 2. Сформировать номер чека: ЧЕК-ГГГГММДД-XXX
        QString checkNumber = QString("ЧЕК-%1-%2")
                                  .arg(orderDate.toString("yyyyMMdd"))
                                  .arg(orderNumber, 3, 10, QChar('0'));

        // 3. Подготовить JSON с позициями
        QJsonArray itemsArray;
        for (const QVariant &v : items) {
            QVariantMap m = v.toMap();
            QJsonObject obj;
            obj["name"] = m["name"].toString();
            obj["quantity"] = m["quantity"].toInt();
            obj["price"] = m["price"].toDouble();
            itemsArray.append(obj);
        }
        QJsonDocument doc(itemsArray);
        QString checkData = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));

        // 4. Вставить запись в Checks
        QSqlQuery insertCheck;
        insertCheck.prepare("INSERT INTO Checks (order_id, check_number, order_number, total_sum, check_data) "
                            "VALUES (:oid, :cnum, :onum, :sum, :data)");
        insertCheck.bindValue(":oid", OrderId);
        insertCheck.bindValue(":cnum", checkNumber);
        insertCheck.bindValue(":onum", orderNumber);
        insertCheck.bindValue(":sum", totalSum);
        insertCheck.bindValue(":data", checkData);
        if (!insertCheck.exec())
            throw DatabaseEx("Ошибка сохранения чека: " + insertCheck.lastError().text());

        QSqlQuery updateOrder;
        updateOrder.prepare("UPDATE Orders SET status = 'Закрыт', TotalCost = :total WHERE OrderId = :id");
        updateOrder.bindValue(":total", totalSum);
        updateOrder.bindValue(":id", OrderId);
        if (!updateOrder.exec())
            throw DatabaseEx("Ошибка обновления статуса заказа: " + updateOrder.lastError().text());

        loger->info("Заказ " + QString::number(OrderId) + " закрыт, чек " + checkNumber);
        emit ordersChanged();
        return true;
    } catch (const DatabaseEx &e) {
        loger->error(QString("closeOrder: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("closeOrder: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка при закрытии заказа");
        return false;
    }
}

QVariantMap DBWorker::getCheckByOrderId(int OrderId)
{
    try {
        if (!db.isOpen()) throw DatabaseEx("База данных не открыта");
        QSqlQuery query;
        query.prepare("SELECT check_id, check_number, order_number, total_sum, check_data, created_at "
                      "FROM Checks WHERE order_id = :oid ORDER BY created_at DESC LIMIT 1");
        query.bindValue(":oid", OrderId);
        if (!query.exec())
            throw DatabaseEx("Ошибка getCheckByOrderId: " + query.lastError().text());
        if (query.next()) {
            QVariantMap check;
            check["checkId"] = query.value("check_id").toInt();
            check["checkNumber"] = query.value("check_number").toString();
            check["orderNumber"] = query.value("order_number").toInt();
            check["totalSum"] = query.value("total_sum").toDouble();
            check["checkData"] = query.value("check_data").toString();
            check["createdAt"] = query.value("created_at").toDateTime();
            return check;
        }
        return QVariantMap();
    } catch (const DatabaseEx &e) {
        loger->error(QString("getCheckByOrderId: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantMap();
    } catch (const std::exception &e) {
        loger->error(QString("getCheckByOrderId: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Ошибка получения чека");
        return QVariantMap();
    }
}

bool DBWorker::syncOrderItems(int OrderId, const QVariantList &newItems)
{
    try {
        if (!db.isOpen()) throw DatabaseEx("База данных не открыта");

        if (OrderId <= 0) {
            loger->error("syncOrderItems: invalid orderId " + QString::number(OrderId));
            emit errorOccurred("Недействительный идентификатор заказа");
            return false;
        }

        QVariantList oldItems = getOrderItems(OrderId);
        if (oldItems.isEmpty() && !newItems.isEmpty()) {
            // Если старых нет, просто добавляем новые – но обычно их нет.
        }

        QMap<int, QVariantMap> oldMap;
        for (const QVariant &v : oldItems) {
            QVariantMap m = v.toMap();
            oldMap[m["foodId"].toInt()] = m;
        }
        QMap<int, QVariantMap> newMap;
        for (const QVariant &v : newItems) {
            QVariantMap m = v.toMap();
            newMap[m["foodId"].toInt()] = m;
        }

        for (auto it = newMap.begin(); it != newMap.end(); ++it) {
            int foodId = it.key();
            int newQty = it.value()["quantity"].toInt();
            if (!oldMap.contains(foodId)) {
                if (!checkAndConsume(foodId, newQty))
                    throw ValidateEx("Не удалось списать ингредиенты для блюда " + QString::number(foodId));
                QSqlQuery insert;
                insert.prepare("INSERT INTO OrderFood (OrderId, FoodId, Quantity, Price) VALUES (:oid, :fid, :qty, :price)");
                insert.bindValue(":oid", OrderId);
                insert.bindValue(":fid", foodId);
                insert.bindValue(":qty", newQty);
                insert.bindValue(":price", it.value()["price"].toDouble());
                if (!insert.exec())
                    throw DatabaseEx("Ошибка добавления позиции: " + insert.lastError().text());
            } else {
                int oldQty = oldMap[foodId]["quantity"].toInt();
                if (newQty != oldQty) {
                    if (!returnIngredients(foodId, oldQty))
                        throw ValidateEx("Ошибка возврата ингредиентов");
                    if (!checkAndConsume(foodId, newQty))
                        throw ValidateEx("Ошибка списания ингредиентов при изменении количества");
                    QSqlQuery update;
                    update.prepare("UPDATE OrderFood SET Quantity = :qty WHERE OrderFoodId = :id");
                    update.bindValue(":qty", newQty);
                    update.bindValue(":id", oldMap[foodId]["orderFoodId"].toInt());
                    if (!update.exec())
                        throw DatabaseEx("Ошибка обновления количества: " + update.lastError().text());
                }
            }
        }

        for (auto it = oldMap.begin(); it != oldMap.end(); ++it) {
            int foodId = it.key();
            if (!newMap.contains(foodId)) {
                int oldQty = it.value()["quantity"].toInt();
                if (!returnIngredients(foodId, oldQty))
                    throw ValidateEx("Ошибка возврата ингредиентов при удалении блюда");
                QSqlQuery del;
                del.prepare("DELETE FROM OrderFood WHERE OrderFoodId = :id");
                del.bindValue(":id", it.value()["orderFoodId"].toInt());
                if (!del.exec())
                    throw DatabaseEx("Ошибка удаления позиции: " + del.lastError().text());
            }
        }

        QSqlQuery sumQuery;
        sumQuery.prepare("SELECT COALESCE(SUM(Quantity * Price), 0) FROM OrderFood WHERE OrderId = :oid");
        sumQuery.bindValue(":oid", OrderId);
        if (!sumQuery.exec() || !sumQuery.next())
            throw DatabaseEx("Ошибка пересчёта суммы");
        double newTotal = sumQuery.value(0).toDouble();
        QSqlQuery updateTotal;
        updateTotal.prepare("UPDATE Orders SET TotalCost = :total WHERE OrderId = :id");
        updateTotal.bindValue(":total", newTotal);
        updateTotal.bindValue(":id", OrderId);
        if (!updateTotal.exec())
            throw DatabaseEx("Ошибка обновления суммы заказа");

        QSqlQuery updateStatus;
        updateStatus.prepare("UPDATE Orders SET status = 'Готовится' WHERE OrderId = :id AND status != 'Закрыт'");
        updateStatus.bindValue(":id", OrderId);
        updateStatus.exec();

        loger->info("Заказ " + QString::number(OrderId) + " синхронизирован, новая сумма " + QString::number(newTotal));
        return true;
    } catch (const DatabaseEx &e) {
        loger->error(QString("syncOrderItems: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const ValidateEx &e) {
        loger->warning(QString("syncOrderItems: ") + e.what());
        emit errorOccurred(e.what());
        return false;
    } catch (const std::exception &e) {
        loger->error(QString("syncOrderItems: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при синхронизации заказа");
        return false;
    }
}

QVariantList DBWorker::getAllOrdersForReport()
{
    return getOrders(QString());
}

QVariantList DBWorker::getClosedOrders()
{
    return getOrders("Закрыт");
}

void DBWorker::updateConnectionParams(const QString &host, int port, const QString &dbName, const QString &user, const QString &password)
{
    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(password);
    loger->info("Параметры подключения к БД обновлены");
}

QVariantList DBWorker::getChecksByDateRange(const QString &from, const QString &to)
{
    try {
        if (!db.isOpen())
            throw DatabaseEx("База данных не открыта");

        QVariantList result;
        QDate fromDate = QDate::fromString(from, "yyyy-MM-dd");
        QDate toDate = QDate::fromString(to, "yyyy-MM-dd");
        if (!fromDate.isValid() || !toDate.isValid())
            throw ValidateEx("Неверный формат даты. Используйте ГГГГ-ММ-ДД");

        QSqlQuery query;
        query.prepare(
            "SELECT c.check_number, c.order_number, c.total_sum, c.created_at, "
            "       o.TableNumber, u.FullName AS waiterName, o.OrderId "
            "FROM Checks c "
            "JOIN Orders o ON c.order_id = o.OrderId "
            "LEFT JOIN Users u ON o.UserId = u.UserId "
            "WHERE DATE(c.created_at) BETWEEN :from AND :to "
            "ORDER BY c.created_at DESC"
            );
        query.bindValue(":from", fromDate);
        query.bindValue(":to", toDate);
        if (!query.exec())
            throw DatabaseEx("Ошибка getChecksByDateRange: " + query.lastError().text());

        while (query.next()) {
            QVariantMap check;
            check["checkNumber"] = query.value("check_number").toString();
            check["orderNumber"] = query.value("order_number").toInt();
            check["totalSum"] = query.value("total_sum").toDouble();
            check["createdAt"] = query.value("created_at").toDateTime();
            check["tableNumber"] = query.value("TableNumber").toInt();
            check["waiterName"] = query.value("waiterName").toString();
            check["orderId"] = query.value("OrderId").toInt();   // для просмотра чека
            result.append(check);
        }
        loger->info("getChecksByDateRange: найдено " + QString::number(result.size()) + " чеков");
        return result;
    }
    catch (const DatabaseEx &e) {
        loger->error(QString("getChecksByDateRange: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    }
    catch (const ValidateEx &e) {
        loger->warning(QString("getChecksByDateRange: ") + e.what());
        emit errorOccurred(e.what());
        return QVariantList();
    }
    catch (const std::exception &e) {
        loger->error(QString("getChecksByDateRange: неизвестная ошибка - ") + e.what());
        emit errorOccurred("Внутренняя ошибка при загрузке чеков");
        return QVariantList();
    }
}
