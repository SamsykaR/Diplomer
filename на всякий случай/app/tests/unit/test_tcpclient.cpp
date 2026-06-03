#include "test_tcpclient.h"
#include "tcpclient.h"
#include "logs.h"

void TestTcpClient::testConnectionTimeout()
{
    Logs logger;
    TcpClient client(&logger);
    bool connected = client.connectToServer("192.0.2.1", 12345);
    QVERIFY(!connected);
    QVERIFY(!client.isConnected());
}

void TestTcpClient::testSendRequestWithoutConnection()
{
    Logs logger;
    TcpClient client(&logger);
    QVERIFY(!client.isConnected());

    bool result = client.checkUser("test", "test");
    QVERIFY(!result);
}
