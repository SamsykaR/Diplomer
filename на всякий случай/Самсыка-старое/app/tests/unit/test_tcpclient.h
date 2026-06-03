#ifndef TEST_TCPCLIENT_H
#define TEST_TCPCLIENT_H

#include <QtTest>

class TestTcpClient : public QObject
{
    Q_OBJECT
private slots:
    void testConnectionTimeout();
    void testSendRequestWithoutConnection();
};

#endif // TEST_TCPCLIENT_H
