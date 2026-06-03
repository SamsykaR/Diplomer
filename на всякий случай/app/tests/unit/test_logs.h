#ifndef TEST_LOGS_H
#define TEST_LOGS_H

#include <QtTest>

class TestLogs : public QObject
{
    Q_OBJECT
private slots:
    void testInfoLog();
    void testWarningLog();
    void testErrorLog();
    void testSetLogPath();

private:
    QString tempLogPath;   // необходимо для тестов
};

#endif // TEST_LOGS_H
