#ifndef DBWORKER_H
#define DBWORKER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include "logs.h"
#include "exceptions.h"

class DBWorker : public QObject
{
    Q_OBJECT
public:
    DBWorker(Logs *logger, QObject *parent = nullptr);
    ~DBWorker();

    Q_INVOKABLE bool checkUser(const QString &login, const QString &password);
    Q_INVOKABLE QString getUserName() const { return userName; }
    Q_INVOKABLE QString getUserRole() const { return userRole; }
    Q_INVOKABLE int getUserId() const { return userId; }
    Q_INVOKABLE bool checkAndConsume(int foodId, int quantity);
    Q_INVOKABLE bool returnIngredients(int foodId, int quantity);
    Q_INVOKABLE bool refreshAllFoodsAvailability();
    Q_INVOKABLE bool reconnect(const QString &host, int port, const QString &dbName, const QString &user, const QString &password);
    Q_INVOKABLE QStringList getLocalIps();
    Q_INVOKABLE bool pingServer(const QString &host, int port);
    Q_INVOKABLE QVariantList getFoods();
    Q_INVOKABLE QVariantList getProducts();
    Q_INVOKABLE QVariantList getDishesList();
    Q_INVOKABLE QVariantList getWaitersList();
    Q_INVOKABLE bool updateProductQuantity(int productId, int newQuantity);
    Q_INVOKABLE bool addProductQuantity(int productId, int delta);
    Q_INVOKABLE int createOrder(int tableNumber, int userId, const QString &specialRequests, double totalCost);
    Q_INVOKABLE bool addOrderItem(int orderId, int foodId, int quantity, double price);
    Q_INVOKABLE QVariantMap getReportRevenue(const QString &from, const QString &to,
                                             int dishId, int tableNumber, int waiterId);
    Q_INVOKABLE QVariantList getRevenueByDay(const QString &from, const QString &to,
                                             int dishId, int tableNumber, int waiterId);
    Q_INVOKABLE QVariantList getFoodPopularity(const QString &from, const QString &to);
    Q_INVOKABLE QVariantList getReportLowStock();
    Q_INVOKABLE QVariantList getLowStockWarning(int foodId, int quantity);

signals:
    void errorOccurred(const QString &message); // для передачи ошибок в QML
    void foodAvailabilityChanged();

private:
    QSqlDatabase db;
    QString userName;
    QString userRole;
    int userId;
    Logs *loger;
};

#endif // DBWORKER_H
