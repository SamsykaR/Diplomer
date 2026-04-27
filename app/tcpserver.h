#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include "logs.h"
#include "exceptions.h"

class DBWorker;

class ClientConnection : public QObject
{
    Q_OBJECT
public:
    ClientConnection(Logs *logger, qintptr socketDescriptor, DBWorker *dbWorker, QObject *parent = nullptr);
    ~ClientConnection();

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    void sendResponse(int requestId, bool success, const QVariantMap &data = QVariantMap());
    QTcpSocket *socket;
    DBWorker *dbWorker;
    QByteArray buffer;
    Logs *loger;
};

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(Logs *logger, DBWorker *dbWorker, QObject *parent = nullptr);
    ~TcpServer();

    bool start(quint16 port);
    void stop();

private slots:
    void onNewConnection();

private:
    QTcpServer *server;
    DBWorker *dbWorker;
    QList<ClientConnection*> connections;
    Logs *loger;
};

#endif // TCPSERVER_H
