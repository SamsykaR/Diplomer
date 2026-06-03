#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include "logs.h"
#include "exceptions.h"

class DBWorker;

/*!
 * \brief Обработчик одного клиентского TCP‑соединения.
 *
 * Принимает JSON‑команды, вызывает соответствующие методы DBWorker
 * и отправляет обратно ответ.
 */
class ClientConnection : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Конструктор.
     * \param logger Логгер.
     * \param socketDescriptor Дескриптор сокета.
     * \param dbWorker Указатель на объект работы с БД.
     * \param parent Родительский объект.
     */
    ClientConnection(Logs *logger, qintptr socketDescriptor, DBWorker *dbWorker, QObject *parent = nullptr);
    ~ClientConnection();

private slots:
    void onReadyRead();      //!< Обработка поступивших данных.
    void onDisconnected();   //!< Удаление при отключении.

private:
    /*!
     * \brief Отправляет JSON‑ответ клиенту.
     * \param requestId ID запроса.
     * \param success Флаг успеха.
     * \param data Дополнительные данные.
     */
    void sendResponse(int requestId, bool success, const QVariantMap &data = QVariantMap());

    QTcpSocket *socket;    //!< Сокет соединения.
    DBWorker *dbWorker;    //!< Указатель на DBWorker.
    QByteArray buffer;     //!< Буфер для сборки JSON.
    Logs *loger;           //!< Логгер.
};

/*!
 * \brief TCP‑сервер, принимающий подключения от клиентов.
 *
 * При новом подключении создаёт ClientConnection и передаёт ему DBWorker.
 */
class TcpServer : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Конструктор.
     * \param logger Логгер.
     * \param dbWorker Указатель на DBWorker.
     * \param parent Родительский объект.
     */
    explicit TcpServer(Logs *logger, DBWorker *dbWorker, QObject *parent = nullptr);
    ~TcpServer();

    /*!
     * \brief Запускает прослушивание порта.
     * \param port Порт.
     * \return true при успехе.
     */
    bool start(quint16 port);
    //! Останавливает сервер и удаляет все соединения.
    void stop();

private slots:
    void onNewConnection(); //!< Обработчик входящего подключения.

private:
    QTcpServer *server;          //!< Объект TCP‑сервера.
    DBWorker *dbWorker;          //!< DBWorker для обработки запросов.
    QList<ClientConnection*> connections; //!< Активные соединения.
    Logs *loger;                 //!< Логгер.
};

#endif // TCPSERVER_H
