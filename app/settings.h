#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QString>
#include "logs.h"
#include "exceptions.h"

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString dbHost READ dbHost WRITE setDbHost NOTIFY dbHostChanged)
    Q_PROPERTY(int dbPort READ dbPort WRITE setDbPort NOTIFY dbPortChanged)
    Q_PROPERTY(QString dbName READ dbName WRITE setDbName NOTIFY dbNameChanged)
    Q_PROPERTY(QString dbUser READ dbUser WRITE setDbUser NOTIFY dbUserChanged)
    Q_PROPERTY(QString dbPassword READ dbPassword WRITE setDbPassword NOTIFY dbPasswordChanged)
    Q_PROPERTY(bool isServerMode READ isServerMode WRITE setIsServerMode NOTIFY isServerModeChanged)
    Q_PROPERTY(QString logPath READ logPath WRITE setLogPath NOTIFY logPathChanged)
    Q_PROPERTY(QString serverHost READ serverHost WRITE setServerHost NOTIFY serverHostChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)

public:
    explicit Settings(Logs *logger, QObject *parent = nullptr);

    QString dbHost() const { return m_dbHost; }
    void setDbHost(const QString &host);

    int dbPort() const { return m_dbPort; }
    void setDbPort(int port);

    QString dbName() const { return m_dbName; }
    void setDbName(const QString &name);

    QString dbUser() const { return m_dbUser; }
    void setDbUser(const QString &user);

    QString dbPassword() const { return m_dbPassword; }
    void setDbPassword(const QString &password);

    bool isServerMode() const { return m_isServerMode; }
    void setIsServerMode(bool mode);

    QString logPath() const { return m_logPath; }
    void setLogPath(const QString &path);

    QString serverHost() const { return m_serverHost; }
    void setServerHost(const QString &host);
    int serverPort() const { return m_serverPort; }
    void setServerPort(int port);

    Q_INVOKABLE void load();
    Q_INVOKABLE void save();

signals:
    void dbHostChanged();
    void dbPortChanged();
    void dbNameChanged();
    void dbUserChanged();
    void dbPasswordChanged();
    void isServerModeChanged();
    void logPathChanged();
    void serverHostChanged();
    void serverPortChanged();

private:
    QString m_dbHost;
    int m_dbPort;
    QString m_dbName;
    QString m_dbUser;
    QString m_dbPassword;
    bool m_isServerMode;
    QString m_logPath;
    Logs *loger;
    QString m_serverHost;
    int m_serverPort;
};

#endif // SETTINGS_H
