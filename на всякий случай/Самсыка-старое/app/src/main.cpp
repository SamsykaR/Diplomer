#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QDirIterator>
#include "dbworker.h"
#include "logs.h"
#include "settings.h"
#include "tcpclient.h"
#include "tcpserver.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    app.setWindowIcon(QIcon(":/resources/icons/logo.png"));

    Logs logger;
    Logs *loger = &logger;

    Settings set(loger);
    engine.rootContext()->setContextProperty("appSettings", &set);

    loger->setLogPath(set.logPath());
    loger->info("Запуск приложения");
    engine.rootContext()->setContextProperty("loger", loger);
    loger->info("Настройки загружены");

    QSettings qsettings("RestaurantApp", "Settings");
    bool serverMode = qsettings.value("isServerMode", false).toBool();
    loger->info("Режим сервера: " + QString::number(serverMode));

    if (serverMode) {
        loger->info("Запуск в серверном режиме");
        DBWorker *worker = new DBWorker(loger, &app);
        engine.rootContext()->setContextProperty("dbWorker", worker);
        loger->info("DBWorker создан");

        TcpServer *tcpServer = new TcpServer(loger, worker, &app);
        quint16 port = static_cast<quint16>(set.serverPort());
        if (!tcpServer->start(port)) {
            loger->error("Не удалось запустить сервер");
            delete tcpServer;
            delete worker;
            return -1;
        }
        loger->info("Сервер запущен на порту " + QString::number(port));
    } else {
        loger->info("Запуск в клиентском режиме");
        TcpClient *client = new TcpClient(loger, &app);
        engine.rootContext()->setContextProperty("dbWorker", client);

        QString host = set.serverHost();
        quint16 port = static_cast<quint16>(set.serverPort());
        if (!client->connectToServer(host, port)) {
            loger->error("Не удалось подключиться к серверу");
        } else {
            loger->info("Подключено к серверу " + host + ":" + QString::number(port));
        }
    }

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.loadFromModule("app", "Main");
    loger->info("QML загружен");

    return app.exec();
}
