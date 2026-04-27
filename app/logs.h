#ifndef LOGS_H
#define LOGS_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include "exceptions.h"


class Logs : public QObject
{
    Q_OBJECT
public:
    explicit Logs(QObject *parent = nullptr);
    ~Logs();

    Q_INVOKABLE void info(const QString &msg);
    Q_INVOKABLE void warning(const QString &msg);
    Q_INVOKABLE void error(const QString &msg);
    Q_INVOKABLE void setLogPath(const QString &path);

private:
    void writeLog(const QString &level, const QString &message);
    void openLogFile();

    QFile logFile;
    QString m_logPath;
    QTextStream out;
};

#endif // LOGS_H
