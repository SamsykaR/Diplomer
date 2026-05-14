#include "test_logs.h"
#include <QFile>
#include <QDir>
#include <QTemporaryFile>
#include "logs.h"

void TestLogs::testInfoLog()
{
    QTemporaryFile tempFile;
    tempFile.open();
    tempLogPath = tempFile.fileName();
    tempFile.close();

    Logs logger;
    logger.setLogPath(tempLogPath);
    logger.info("Test info message");

    QFile file(tempLogPath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QByteArray content = file.readAll();
    QVERIFY(content.contains("Test info message"));
    QVERIFY(content.contains("[INF]"));
    file.close();
    QFile::remove(tempLogPath);
}

void TestLogs::testWarningLog()
{
    QTemporaryFile tempFile;
    tempFile.open();
    tempLogPath = tempFile.fileName();
    tempFile.close();

    Logs logger;
    logger.setLogPath(tempLogPath);
    logger.warning("Test warning message");

    QFile file(tempLogPath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QByteArray content = file.readAll();
    QVERIFY(content.contains("Test warning message"));
    QVERIFY(content.contains("[WARN]"));
    file.close();
    QFile::remove(tempLogPath);
}

void TestLogs::testErrorLog()
{
    QTemporaryFile tempFile;
    tempFile.open();
    tempLogPath = tempFile.fileName();
    tempFile.close();

    Logs logger;
    logger.setLogPath(tempLogPath);
    logger.error("Test error message");

    QFile file(tempLogPath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QByteArray content = file.readAll();
    QVERIFY(content.contains("Test error message"));
    QVERIFY(content.contains("[ERR]"));
    file.close();
    QFile::remove(tempLogPath);
}

void TestLogs::testSetLogPath()
{
    Logs logger;
    QString newPath = QDir::temp().absoluteFilePath("custom_log_test.txt");
    logger.setLogPath(newPath);
    logger.info("Custom path test");

    QFile file(newPath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QByteArray content = file.readAll();
    QVERIFY(content.contains("Custom path test"));
    file.close();
    QFile::remove(newPath);
}
