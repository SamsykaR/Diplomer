#include "test_settings.h"
#include "settings.h"
#include "logs.h"

void TestSettings::testSetAndGet()
{
    Logs logger;
    Settings settings(&logger);
    settings.setDbHost("192.168.1.100");
    QCOMPARE(settings.dbHost(), QString("192.168.1.100"));

    settings.setDbPort(9999);
    QCOMPARE(settings.dbPort(), 9999);

    settings.setIsServerMode(true);
    QCOMPARE(settings.isServerMode(), true);

    settings.setServerHost("test-server");
    QCOMPARE(settings.serverHost(), QString("test-server"));

    settings.setServerPort(12345);
    QCOMPARE(settings.serverPort(), 12345);
}

void TestSettings::testSaveAndLoad()
{
    Logs logger;
    Settings settings(&logger);
    settings.setDbHost("saved_host_test");
    settings.setDbPort(8888);
    settings.setServerHost("saved_server");
    settings.setServerPort(54321);
    settings.save();

    Settings loaded(&logger);
    loaded.load();
    QCOMPARE(loaded.dbHost(), QString("saved_host_test"));
    QCOMPARE(loaded.dbPort(), 8888);
    QCOMPARE(loaded.serverHost(), QString("saved_server"));
    QCOMPARE(loaded.serverPort(), 54321);

    Settings restore(&logger);
    restore.load();
    restore.save();
}
