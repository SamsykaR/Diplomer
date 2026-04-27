#include "logs.h"
#include <QDateTime>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

Logs::Logs(QObject *parent) : QObject(parent), m_logPath("logs.txt")
{
    openLogFile();
}

Logs::~Logs()
{
    if (logFile.isOpen())
        logFile.close();
}

void Logs::setLogPath(const QString &path)
{
    if (m_logPath == path) return;
    m_logPath = path;
    openLogFile();
}

void Logs::openLogFile()
{
    if (logFile.isOpen())
        logFile.close();

    QString actualPath = m_logPath;
    QFileInfo info(actualPath);
    if (info.isRelative()) {
        actualPath = QDir::current().absoluteFilePath(actualPath);
    }

    logFile.setFileName(actualPath);
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        out.setDevice(&logFile);
        writeLog("INF", "Лог-файл открыт: " + actualPath);
        return;
    }

    QString fallbackDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    if (fallbackDir.isEmpty())
        fallbackDir = QDir::tempPath();
    else
        QDir().mkpath(fallbackDir);

    QString fallbackPath = fallbackDir + "/logs.txt";
    logFile.setFileName(fallbackPath);
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        out.setDevice(&logFile);
        writeLog("WARN", "Не удалось открыть лог по пути " + actualPath + ", используется " + fallbackPath);
    } else {
        qWarning() << "Критическая ошибка: не удалось открыть лог-файл даже в системной папке";
    }
}

void Logs::writeLog(const QString &level, const QString &message)
{
    if (!logFile.isOpen()) return;
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString line = QString("%1 [%2] %3\n").arg(timestamp, level, message);
    out << line;
    out.flush();

    if (level == "ERR") qCritical() << message;
    else if (level == "WARN") qWarning() << message;
    else qInfo() << message;
}

void Logs::info(const QString &msg)    { writeLog("INF", msg); }
void Logs::warning(const QString &msg) { writeLog("WARN", msg); }
void Logs::error(const QString &msg)   { writeLog("ERR", msg); }
