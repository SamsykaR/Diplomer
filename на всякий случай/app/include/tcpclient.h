#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QVariantList>
#include <QVariantMap>
#include "logs.h"
#include "exceptions.h"

/*!
 * \brief Клиент для удалённого взаимодействия с сервером приложения.
 *
 * Устанавливает TCP‑соединение, обменивается JSON‑сообщениями.
 * Дублирует интерфейс DBWorker для прозрачной работы из QML
 * независимо от режима (локальный или клиент‑сервер).
 * При потере связи генерирует сигнал errorOccurred.
 */
class TcpClient : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Конструктор.
     * \param logger Указатель на логгер.
     * \param parent Родительский объект.
     */
    explicit TcpClient(Logs *logger, QObject *parent = nullptr);
    ~TcpClient();

    /*!
     * \brief Подключается к серверу.
     * \param host Адрес сервера.
     * \param port Порт.
     * \return true, если соединение установлено.
     */
    bool connectToServer(const QString &host, quint16 port);
    //! Закрывает соединение и очищает буфер.
    void disconnectFromServer();
    //! \return true, если сокет находится в состоянии ConnectedState.
    bool isConnected() const;

    /*!
     * \brief Отправляет на сервер запрос аутентификации.
     * \param login Логин.
     * \param password Пароль.
     * \return true при успешной проверке.
     */
    Q_INVOKABLE bool checkUser(const QString &login, const QString &password);

    //! \return Имя текущего пользователя.
    Q_INVOKABLE QString getUserName() const;
    //! \return Роль текущего пользователя.
    Q_INVOKABLE QString getUserRole() const;
    //! \return ID текущего пользователя.
    Q_INVOKABLE int getUserId() const;

    /*!
     * \brief Запрашивает меню с сервера.
     * \return Список блюд (QVariantList).
     */
    Q_INVOKABLE QVariantList getFoods();

    /*!
     * \brief Запрашивает список продуктов.
     * \return Список продуктов.
     */
    Q_INVOKABLE QVariantList getProducts();

    /*!
     * \brief Отправляет команду установки абсолютного количества продукта.
     * \param productId ID продукта.
     * \param newQuantity Новое количество.
     * \return true при успехе.
     */
    Q_INVOKABLE bool updateProductQuantity(int productId, int newQuantity);

    /*!
     * \brief Отправляет команду изменения количества продукта на дельту.
     * \param productId ID продукта.
     * \param delta Изменение (может быть отрицательным).
     * \return true при успехе.
     */
    Q_INVOKABLE bool addProductQuantity(int productId, int delta);

    /*!
     * \brief Создаёт заказ на сервере.
     * \param tableNumber Номер столика.
     * \param userId ID пользователя.
     * \param specialRequests Комментарий.
     * \param totalCost Общая стоимость.
     * \return ID созданного заказа или -1.
     */
    Q_INVOKABLE int createOrder(int tableNumber, int userId, const QString &specialRequests, double totalCost);

    /*!
     * \brief Добавляет позицию заказа.
     * \param orderId ID заказа.
     * \param foodId ID блюда.
     * \param quantity Количество.
     * \param price Цена.
     * \return true при успехе.
     */
    Q_INVOKABLE bool addOrderItem(int orderId, int foodId, int quantity, double price);

    /*!
     * \brief Запрашивает проверку и списание ингредиентов.
     * \param foodId ID блюда.
     * \param quantity Количество порций.
     * \return true, если списание разрешено и выполнено.
     */
    Q_INVOKABLE bool checkAndConsume(int foodId, int quantity);

    /*!
     * \brief Запрашивает возврат ингредиентов на склад.
     * \param foodId ID блюда.
     * \param quantity Количество порций.
     * \return true при успехе.
     */
    Q_INVOKABLE bool returnIngredients(int foodId, int quantity);

    /*!
     * \brief Получает предупреждения о снижении остатков ниже порога.
     * \param foodId ID блюда.
     * \param quantity Количество порций.
     * \return Список предупреждений.
     */
    Q_INVOKABLE QVariantList getLowStockWarning(int foodId, int quantity);

    /*!
     * \brief Запрашивает отчёт по общей выручке.
     * \param from Начальная дата.
     * \param to Конечная дата.
     * \param dishId ID блюда (0 – все).
     * \param tableNumber № столика (0 – все).
     * \param waiterId ID официанта (0 – все).
     * \return Словарь с ключом "totalRevenue" или "error".
     */
    Q_INVOKABLE QVariantMap getReportRevenue(const QString &from, const QString &to, int dishId, int tableNumber, int waiterId);

    /*!
     * \brief Запрашивает выручку по дням.
     * \param from Начальная дата.
     * \param to Конечная дата.
     * \param dishId ID блюда.
     * \param tableNumber № столика.
     * \param waiterId ID официанта.
     * \return Список словарей с ключами "date" и "total".
     */
    Q_INVOKABLE QVariantList getRevenueByDay(const QString &from, const QString &to, int dishId, int tableNumber, int waiterId);

    /*!
     * \brief Запрашивает популярность блюд.
     * \param from Начальная дата.
     * \param to Конечная дата.
     * \return Список словарей с ключами "foodName" и "count".
     */
    Q_INVOKABLE QVariantList getFoodPopularity(const QString &from, const QString &to);

    //! Запрашивает отчёт по продуктам с низким остатком.
    Q_INVOKABLE QVariantList getReportLowStock();

    //! Запрашивает краткий список блюд (ID и название).
    Q_INVOKABLE QVariantList getDishesList();

    //! Запрашивает список официантов.
    Q_INVOKABLE QVariantList getWaitersList();

    //! Возвращает IP-адреса локального устройства.
    Q_INVOKABLE QStringList getLocalIps();

    /*!
     * \brief Проверяет доступность удалённого хоста через пробное TCP-подключение.
     * \param host Адрес.
     * \param port Порт.
     * \return true, если соединение удалось установить за 2 с.
     */
    Q_INVOKABLE bool pingServer(const QString &host, int port);

    /*!
     * \brief Переподключается к серверу (сначала отключается, затем подключается заново).
     * \param host Новый адрес.
     * \param port Новый порт.
     * \return true при успешном подключении.
     */
    Q_INVOKABLE bool reconnect(const QString &host, quint16 port);

    /*!
     * \brief Отправляет запрос на сохранение отчёта в CSV на сервер.
     * \param reportType Тип отчёта.
     * \param from Начальная дата.
     * \param to Конечная дата.
     * \param dishId ID блюда (0 – все).
     * \param tableNumber № столика (0 – все).
     * \param waiterId ID официанта (0 – все).
     * \param fileName Полный путь к сохраняемому файлу.
     * \return true, если сервер успешно создал файл.
     */
    Q_INVOKABLE bool saveReportToCSV(const QString &reportType,
                                     const QString &from,
                                     const QString &to,
                                     int dishId,
                                     int tableNumber,
                                     int waiterId,
                                     const QString &fileName);

signals:
    //! Сообщение об ошибке для UI.
    void errorOccurred(const QString &message);
    //! Уведомление от сервера об изменении данных (data_changed).
    void dataChanged();
    //! Пришёл ответ на ранее отправленный запрос.
    void responseReceived(int requestId, bool success, const QVariantMap &data);

private:
    /*!
     * \brief Формирует и отправляет JSON-сообщение.
     * \param command Команда.
     * \param data Параметры (опционально).
     * \return ID запроса или -1 при ошибке.
     */
    int sendRequest(const QString &command, const QVariantMap &data = QVariantMap());

    /*!
     * \brief Ожидает ответа с указанным ID (с таймаутом 60 с).
     * \param requestId ID запроса.
     * \param resultData Сюда будет помещён ответ.
     * \return true, если ответ получен и success == true.
     */
    bool waitForResponse(int requestId, QVariantMap &resultData);

    QTcpSocket *socket;              //!< TCP‑сокет.
    QByteArray buffer;              //!< Буфер для накопления фрагментов JSON.
    int nextRequestId;              //!< Счётчик для генерации ID запросов.
    QMap<int, QVariantMap> pendingResponses; //!< Ожидаемые ответы (ключ – requestId).

    QString userName;               //!< Имя текущего пользователя.
    QString userRole;               //!< Роль текущего пользователя.
    int userId;                     //!< ID текущего пользователя.
    Logs *loger;                    //!< Логгер.
};

#endif // TCPCLIENT_H
