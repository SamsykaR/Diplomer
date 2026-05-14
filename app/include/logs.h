#ifndef LOGS_H
#define LOGS_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include "exceptions.h"

/*!
 * \brief Сервис логирования сообщений в файл.
 *
 * Записывает сообщения с меткой времени и уровнем (INF, WARN, ERR).
 * Если основной файл не может быть открыт, использует резервный путь
 * в директории AppLocalDataLocation.
 */
class Logs : public QObject
{
    Q_OBJECT
public:
    explicit Logs(QObject *parent = nullptr);
    ~Logs();

    //! Записывает информационное сообщение.
    Q_INVOKABLE void info(const QString &msg);
    //! Записывает предупреждение.
    Q_INVOKABLE void warning(const QString &msg);
    //! Записывает сообщение об ошибке.
    Q_INVOKABLE void error(const QString &msg);
    /*!
     * \brief Устанавливает путь к лог-файлу и переоткрывает его.
     * \param path Новый путь.
     */
    Q_INVOKABLE void setLogPath(const QString &path);

private:
    void writeLog(const QString &level, const QString &message); //!< Форматирует и записывает строку лога.
    void openLogFile();                                          //!< Открывает или переоткрывает файл лога.

    QFile logFile;        //!< Файловый объект.
    QString m_logPath;    //!< Текущий путь к файлу.
    QTextStream out;      //!< Поток для записи.
};

#endif // LOGS_H
