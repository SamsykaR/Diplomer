#include "test_logs.h"
#include "test_settings.h"
#include "test_tcpclient.h"

int main(int argc, char *argv[])
{
    int status = 0;
    status |= QTest::qExec(new TestLogs, argc, argv);
    status |= QTest::qExec(new TestSettings, argc, argv);
    status |= QTest::qExec(new TestTcpClient, argc, argv);
    return status;
}
