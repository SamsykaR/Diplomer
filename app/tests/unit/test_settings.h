#ifndef TEST_SETTINGS_H
#define TEST_SETTINGS_H

#include <QtTest>

class TestSettings : public QObject
{
    Q_OBJECT
private slots:
    void testSetAndGet();
    void testSaveAndLoad();
};

#endif // TEST_SETTINGS_H
