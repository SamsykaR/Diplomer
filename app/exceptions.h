#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QString>
#include <stdexcept>

class Exceptions : public std::exception
{
public:
    explicit Exceptions(const QString &message);
    virtual const char* what() const noexcept override;

private:
    std::string msg;
};

class ValidateEx : public Exceptions
{
public:
    explicit ValidateEx(const QString &message) : Exceptions(message) {}
};

class DatabaseEx : public Exceptions
{
public:
    explicit DatabaseEx(const QString &message) : Exceptions(message) {}
};

class NetworkEx : public Exceptions
{
public:
    explicit NetworkEx(const QString &message) : Exceptions(message) {}
};

#endif // EXCEPTIONS_H
