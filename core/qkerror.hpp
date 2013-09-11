#pragma once

#include "qkcore.export.hpp"

#include <QString>
#include <QCoreApplication>

#define ERRLOC qApp->translate("", "Файл: %1:%2, Функция: %3").arg(__FILE__).arg(__LINE__).arg(__PRETTY_FUNCTION__)

class QKCORE_EXPORT QkError
{
public:
    QkError()
    {
    }

    QkError(const QString& pLocation, const QString& pMessage)
        : mMessage(pMessage), mLocation(pLocation)
    {
    }

    bool hasError() const { return !mMessage.isEmpty() || !mLocation.isEmpty(); }
    QString message() const { return mMessage; }
    QString location() const { return mLocation; }
    QString toString() const { return QString("Error at %1. Message: %2").arg(mLocation).arg(mMessage); }

private:
    QString mMessage;
    QString mLocation;
};
