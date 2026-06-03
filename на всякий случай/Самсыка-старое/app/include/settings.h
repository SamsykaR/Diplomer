#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QString>
#include <QDir>
#include "logs.h"
#include "exceptions.h"

/*!
 * \brief Класс для хранения и управления настройками приложения.
 *
 * Обёртка над QSettings, предоставляющая свойства QML
 * для доступа к параметрам базы данных, сети и путям.
 */
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
    Q_PROPERTY(QString reportsPath READ reportsPath WRITE setReportsPath NOTIFY reportsPathChanged)

public:
    /*!
     * \brief Конструктор. Автоматически вызывает load().
     * \param logger Указатель на логгер.
     * \param parent Родительский объект.
     */
    explicit Settings(Logs *logger, QObject *parent = nullptr);

    // Геттеры и сеттеры с краткими пояснениями
    QString dbHost() const { return m_dbHost; }
    void setDbHost(const QString &host);                     //!< Устанавливает хост БД.

    int dbPort() const { return m_dbPort; }
    void setDbPort(int port);                                //!< Устанавливает порт БД.

    QString dbName() const { return m_dbName; }
    void setDbName(const QString &name);                     //!< Устанавливает имя БД.

    QString dbUser() const { return m_dbUser; }
    void setDbUser(const QString &user);                     //!< Устанавливает пользователя БД.

    QString dbPassword() const { return m_dbPassword; }
    void setDbPassword(const QString &password);             //!< Устанавливает пароль БД.

    bool isServerMode() const { return m_isServerMode; }
    void setIsServerMode(bool mode);                         //!< Включает/выключает серверный режим.

    QString logPath() const { return m_logPath; }
    void setLogPath(const QString &path);                    //!< Задаёт путь к лог-файлу.

    QString serverHost() const { return m_serverHost; }
    void setServerHost(const QString &host);                 //!< Устанавливает IP-адрес сервера.

    int serverPort() const { return m_serverPort; }
    void setServerPort(int port);                            //!< Устанавливает порт сервера.

    QString reportsPath() const { return m_reportsPath; }
    void setReportsPath(const QString &path);                //!< Задаёт каталог для сохранения отчётов.

    //! Загружает все настройки из QSettings.
    Q_INVOKABLE void load();
    //! Сохраняет текущие настройки в QSettings.
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
    void reportsPathChanged();

private:
    QString m_dbHost;         //!< Хост базы данных.
    int m_dbPort;             //!< Порт базы данных.
    QString m_dbName;         //!< Имя базы данных.
    QString m_dbUser;         //!< Пользователь БД.
    QString m_dbPassword;     //!< Пароль БД.
    bool m_isServerMode;      //!< Флаг серверного режима.
    QString m_logPath;        //!< Путь к файлу лога.
    Logs *loger;              //!< Логгер.
    QString m_serverHost;     //!< Адрес TCP‑сервера.
    int m_serverPort;         //!< Порт TCP‑сервера.
    QString m_reportsPath;    //!< Путь к папке отчётов.
};

#endif // SETTINGS_H
