#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QString>
#include <stdexcept>

/*!
 * \brief Базовый класс исключений, принимающий QString.
 *
 * Хранит сообщение в std::string для совместимости с std::exception.
 * Все специализированные исключения наследуют этот класс.
 */
class Exceptions : public std::exception
{
public:
    /*!
     * \brief Конструктор.
     * \param message Описание ошибки.
     */
    explicit Exceptions(const QString &message);
    virtual const char* what() const noexcept override;

private:
    std::string msg; //!< Сохранённое сообщение.
};

//! \brief Исключение валидации (некорректные данные, нехватка продуктов и т.п.).
class ValidateEx : public Exceptions
{
public:
    explicit ValidateEx(const QString &message) : Exceptions(message) {}
};

//! \brief Исключение, связанное с ошибками базы данных.
class DatabaseEx : public Exceptions
{
public:
    explicit DatabaseEx(const QString &message) : Exceptions(message) {}
};

//! \brief Исключение, связанное с сетевыми ошибками.
class NetworkEx : public Exceptions
{
public:
    explicit NetworkEx(const QString &message) : Exceptions(message) {}
};

//! \brief Исключение, связанное с файловыми операциями.
class FileEx : public Exceptions
{
public:
    explicit FileEx(const QString &message) : Exceptions(message) {}
};

#endif // EXCEPTIONS_H
