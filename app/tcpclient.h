#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QVariantList>
#include <QVariantMap>
#include "logs.h"
#include "exceptions.h"

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(Logs *logger, QObject *parent = nullptr);
    ~TcpClient();

    bool connectToServer(const QString &host, quint16 port);
    void disconnectFromServer();
    bool isConnected() const;

    Q_INVOKABLE bool checkUser(const QString &login, const QString &password);
    Q_INVOKABLE QString getUserName() const;
    Q_INVOKABLE QString getUserRole() const;
    Q_INVOKABLE int getUserId() const;
    Q_INVOKABLE QVariantList getFoods();
    Q_INVOKABLE QVariantList getProducts();
    Q_INVOKABLE bool updateProductQuantity(int productId, int newQuantity);
    Q_INVOKABLE bool addProductQuantity(int productId, int delta);
    Q_INVOKABLE int createOrder(int tableNumber, int userId, const QString &specialRequests, double totalCost);
    Q_INVOKABLE bool addOrderItem(int orderId, int foodId, int quantity, double price);
    Q_INVOKABLE bool checkAndConsume(int foodId, int quantity);
    Q_INVOKABLE bool returnIngredients(int foodId, int quantity);
    Q_INVOKABLE QVariantList getLowStockWarning(int foodId, int quantity);
    Q_INVOKABLE QVariantMap getReportRevenue(const QString &from, const QString &to, int dishId, int tableNumber, int waiterId);
    Q_INVOKABLE QVariantList getRevenueByDay(const QString &from, const QString &to, int dishId, int tableNumber, int waiterId);
    Q_INVOKABLE QVariantList getFoodPopularity(const QString &from, const QString &to);
    Q_INVOKABLE QVariantList getReportLowStock();
    Q_INVOKABLE QVariantList getDishesList();
    Q_INVOKABLE QVariantList getWaitersList();
    Q_INVOKABLE QStringList getLocalIps();
    Q_INVOKABLE bool pingServer(const QString &host, int port);
    Q_INVOKABLE bool reconnect(const QString &host, quint16 port);

signals:
    void errorOccurred(const QString &message);
    void dataChanged();
    void responseReceived(int requestId, bool success, const QVariantMap &data);

private:
    int sendRequest(const QString &command, const QVariantMap &data = QVariantMap());
    bool waitForResponse(int requestId, QVariantMap &resultData);

    QTcpSocket *socket;
    QByteArray buffer;
    int nextRequestId;
    QMap<int, QVariantMap> pendingResponses;

    QString userName;
    QString userRole;
    int userId;
    Logs *loger;
};

#endif // TCPCLIENT_H
