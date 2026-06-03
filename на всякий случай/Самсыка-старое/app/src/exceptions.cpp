#include "exceptions.h"

Exceptions::Exceptions(const QString &message)
    : msg(message.toStdString())
{
}

const char* Exceptions::what() const noexcept
{
    return msg.c_str();
}
