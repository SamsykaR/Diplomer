#ifndef DBWORKER_H
#define DBWORKER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QStringConverter>
#include "logs.h"
#include "exceptions.h"

/*!
 * \brief Класс для взаимодействия с базой данных PostgreSQL ресторана.
 *
 * Предоставляет методы аутентификации, работы с меню, складом,
 * заказами и отчётами. Хранит информацию о текущем пользователе.
 */
class DBWorker : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Конструктор.
     * \param logger Указатель на объект логирования.
     * \param parent Родительский объект (по умолчанию nullptr).
     */
    DBWorker(Logs *logger, QObject *parent = nullptr);
    ~DBWorker();

    /*!
     * \brief Проверяет логин и пароль пользователя.
     * \param login Логин.
     * \param password Пароль.
     * \return true при успешной аутентификации.
     */
    Q_INVOKABLE bool checkUser(const QString &login, const QString &password);

    //! \brief Возвращает имя текущего пользователя.
    Q_INVOKABLE QString getUserName() const { return userName; }
    //! \brief Возвращает роль текущего пользователя.
    Q_INVOKABLE QString getUserRole() const { return userRole; }
    //! \brief Возвращает идентификатор текущего пользователя.
    Q_INVOKABLE int getUserId() const { return userId; }

    /*!
     * \brief Проверяет наличие ингредиентов и списывает их со склада.
     * \param foodId ID блюда.
     * \param quantity Количество порций.
     * \return true, если списание прошло успешно.
     */
    Q_INVOKABLE bool checkAndConsume(int foodId, int quantity);

    /*!
     * \brief Возвращает ингредиенты на склад при отмене позиции заказа.
     * \param foodId ID блюда.
     * \param quantity Количество порций.
     * \return true в случае успеха.
     */
    Q_INVOKABLE bool returnIngredients(int foodId, int quantity);

    //! \brief Пересчитывает доступность всех блюд на основе остатков продуктов.
    Q_INVOKABLE bool refreshAllFoodsAvailability();

    /*!
     * \brief Переподключает базу данных с новыми параметрами.
     * \param host Хост.
     * \param port Порт.
     * \param dbName Имя базы данных.
     * \param user Пользователь.
     * \param password Пароль.
     * \return true при успешном подключении.
     */
    Q_INVOKABLE bool reconnect(const QString &host, int port, const QString &dbName, const QString &user, const QString &password);

    //! \brief Возвращает список локальных IPv4‑адресов.
    Q_INVOKABLE QStringList getLocalIps();

    /*!
     * \brief Проверяет доступность удалённого сервера (заглушка, всегда true).
     * \param host Адрес.
     * \param port Порт.
     * \return true.
     */
    Q_INVOKABLE bool pingServer(const QString &host, int port);

    //! \brief Загружает меню с составом и доступностью.
    Q_INVOKABLE QVariantList getFoods();

    //! \brief Загружает складские остатки продуктов.
    Q_INVOKABLE QVariantList getProducts();

    //! \brief Возвращает краткий список блюд (ID и название).
    Q_INVOKABLE QVariantList getDishesList();

    //! \brief Возвращает список официантов.
    Q_INVOKABLE QVariantList getWaitersList();

    /*!
     * \brief Устанавливает новое абсолютное количество продукта.
     * \param productId ID продукта.
     * \param newQuantity Новое количество.
     * \return true при успешном обновлении.
     */
    Q_INVOKABLE bool updateProductQuantity(int productId, int newQuantity);

    /*!
     * \brief Изменяет количество продукта на заданную величину.
     * \param productId ID продукта.
     * \param delta Изменение (может быть отрицательным).
     * \return true при успехе.
     */
    Q_INVOKABLE bool addProductQuantity(int productId, int delta);

    /*!
     * \brief Создаёт новый заказ.
     * \param tableNumber Номер столика.
     * \param userId ID официанта.
     * \param specialRequests Особые пожелания.
     * \param totalCost Итоговая стоимость.
     * \return ID созданного заказа или -1 при ошибке.
     */
    Q_INVOKABLE int createOrder(int tableNumber, int userId, const QString &specialRequests, double totalCost);

    /*!
     * \brief Добавляет позицию в заказ.
     * \param orderId ID заказа.
     * \param foodId ID блюда.
     * \param quantity Количество.
     * \param price Цена за единицу.
     * \return true при успешной вставке.
     */
    Q_INVOKABLE bool addOrderItem(int orderId, int foodId, int quantity, double price);

    /*!
     * \brief Возвращает общую выручку за период с применёнными фильтрами.
     * \param from Начальная дата (ГГГГ-ММ-ДД).
     * \param to Конечная дата.
     * \param dishId ID блюда (0 – без фильтра).
     * \param tableNumber Номер столика (0 – без фильтра).
     * \param waiterId ID официанта (0 – без фильтра).
     * \return Словарь с ключом "totalRevenue" или "error".
     */
    Q_INVOKABLE QVariantMap getReportRevenue(const QString &from, const QString &to,
                                             int dishId, int tableNumber, int waiterId);

    /*!
     * \brief Возвращает выручку, сгруппированную по дням.
     * \param from Начальная дата.
     * \param to Конечная дата.
     * \param dishId Фильтр по блюду (0 – без фильтра).
     * \param tableNumber Фильтр по столику (0 – без фильтра).
     * \param waiterId Фильтр по официанту (0 – без фильтра).
     * \return Список словарей с полями "date" и "total".
     */
    Q_INVOKABLE QVariantList getRevenueByDay(const QString &from, const QString &to,
                                             int dishId, int tableNumber, int waiterId);

    /*!
     * \brief Получает популярность блюд за период.
     * \param from Начальная дата.
     * \param to Конечная дата.
     * \return Список словарей с ключами "foodName" и "count".
     */
    Q_INVOKABLE QVariantList getFoodPopularity(const QString &from, const QString &to);

    //! \brief Возвращает продукты, количество которых ниже минимального порога.
    Q_INVOKABLE QVariantList getReportLowStock();

    /*!
     * \brief Предупреждает о продуктах, остаток которых станет ниже порога после приготовления блюда.
     * \param foodId ID блюда.
     * \param quantity Количество порций.
     * \return Список предупреждений с полями productName, currentQuantity, newQuantity, minThreshold.
     */
    Q_INVOKABLE QVariantList getLowStockWarning(int foodId, int quantity);

    /*!
     * \brief Сохраняет отчёт в CSV‑файл.
     * \param reportType Тип отчёта ("revenue", "popularity", "lowstock").
     * \param from Начальная дата.
     * \param to Конечная дата.
     * \param dishId Фильтр по блюду.
     * \param tableNumber Фильтр по столику.
     * \param waiterId Фильтр по официанту.
     * \param fileName Полный путь к выходному файлу.
     * \return true при успешном сохранении.
     */
    Q_INVOKABLE bool saveReportToCSV(const QString &reportType,
                                     const QString &from,
                                     const QString &to,
                                     int dishId,
                                     int tableNumber,
                                     int waiterId,
                                     const QString &fileName);

signals:
    //! Сигнал, передающий текст ошибки для отображения в интерфейсе.
    void errorOccurred(const QString &message);
    //! Сообщает об изменении доступности блюд (например, после обновления остатков).
    void foodAvailabilityChanged();

private:
    QSqlDatabase db;       //!< Объект подключения к PostgreSQL.
    QString userName;      //!< Имя текущего пользователя.
    QString userRole;      //!< Роль (waiter, storekeeper, admin).
    int userId;            //!< Идентификатор текущего пользователя.
    Logs *loger;           //!< Указатель на логгер.
};

#endif // DBWORKER_H
