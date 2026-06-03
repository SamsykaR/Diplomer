#include "settings.h"
#include <QSettings>

Settings::Settings(Logs *logger, QObject *parent) : QObject(parent), loger(logger)
{
    load();
}

void Settings::setDbHost(const QString &host)
{
    if (m_dbHost == host) return;
    m_dbHost = host;
    emit dbHostChanged();
}

void Settings::setDbPort(int port)
{
    if (m_dbPort == port) return;
    m_dbPort = port;
    emit dbPortChanged();
}

void Settings::setDbName(const QString &name)
{
    if (m_dbName == name) return;
    m_dbName = name;
    emit dbNameChanged();
}

void Settings::setDbUser(const QString &user)
{
    if (m_dbUser == user) return;
    m_dbUser = user;
    emit dbUserChanged();
}

void Settings::setDbPassword(const QString &password)
{
    if (m_dbPassword == password) return;
    m_dbPassword = password;
    emit dbPasswordChanged();
}

void Settings::setReportsPath(const QString &path)
{
    if (m_reportsPath == path) return;
    m_reportsPath = path;
    emit reportsPathChanged();
}

void Settings::load()
{
    QSettings settings("RestaurantApp", "Settings");
    m_dbHost = settings.value("dbHost", "127.0.0.1").toString();
    m_dbPort = settings.value("dbPort", 5432).toInt();
    m_dbName = settings.value("dbName", "RestN").toString();
    m_dbUser = settings.value("dbUser", "R").toString();
    m_dbPassword = settings.value("dbPassword", "").toString();
    m_isServerMode = settings.value("isServerMode", false).toBool();
    m_logPath = settings.value("logPath", "logs.txt").toString();
    m_serverHost = settings.value("serverHost", "127.0.0.1").toString();
    m_serverPort = settings.value("serverPort", 12345).toInt();
    m_reportsPath = settings.value("reportsPath", QDir::current().absoluteFilePath("reports")).toString();
    loger->info("Настройки загружены: хост=" + m_dbHost + ", порт=" + QString::number(m_dbPort));
}

void Settings::save()
{
    QSettings settings("RestaurantApp", "Settings");
    settings.setValue("dbHost", m_dbHost);
    settings.setValue("dbPort", m_dbPort);
    settings.setValue("dbName", m_dbName);
    settings.setValue("dbUser", m_dbUser);
    settings.setValue("dbPassword", m_dbPassword);
    settings.setValue("isServerMode", m_isServerMode);
    settings.setValue("logPath", m_logPath);
    settings.setValue("serverHost", m_serverHost);
    settings.setValue("serverPort", m_serverPort);
    settings.setValue("reportsPath", m_reportsPath);
    loger->info("Настройки сохранены");
}

void Settings::setIsServerMode(bool mode)
{
    if (m_isServerMode == mode) return;
    m_isServerMode = mode;
    emit isServerModeChanged();
}

void Settings::setLogPath(const QString &path)
{
    if (m_logPath == path) return;
    m_logPath = path;
    emit logPathChanged();
}

void Settings::setServerHost(const QString &host)
{
    if (m_serverHost == host) return;
    m_serverHost = host;
    emit serverHostChanged();
}
void Settings::setServerPort(int port)
{
    if (m_serverPort == port) return;
    m_serverPort = port;
    emit serverPortChanged();
}
