#pragma once

#include "core.export.hpp"

#include <QString>
#include <QCoreApplication>

#define ERRLOC qApp->translate("", "Файл: %1:%2, Функция: %3").arg(__FILE__).arg(__LINE__).arg(__PRETTY_FUNCTION__)
#define TR(x) qApp->translate("", x)

#define ASSERTPTR(pName) if (pName == nullptr) throw Qk::Core::ErrorBadArgument(ERRLOC, qApp->translate("Ошибка", "Параметр '%1' не может быть NULL").arg(#pName));

#define ASSERTSTR(pStr, pMin, pMax) if (QString(pStr).size() < pMin || QString(pStr).size() > pMax) \
    throw Qk::Core::ErrorBadArgument(ERRLOC, qApp->translate("Ошибка", "Параметр '%1', со значением '%2', должен быть длиной от %3 до %4 знаков").arg(#pStr).arg(pStr).arg(pMin).arg(pMax));

#define ASSERTMAPUNIQKEY(pMap, pKey) if (pMap.contains(pKey)) \
    throw Qk::Core::ErrorBadArgument(ERRLOC, qApp->translate("Ошибка", "Словарь '%1' уже имеет ключ '%2'").arg(#pMap).arg(pKey));

#define ASSERTMAPUNIQVAL(pMap, pVal) if (pMap.values().contains(pVal)) \
    throw Qk::Core::ErrorBadArgument(ERRLOC, qApp->translate("Ошибка", "Словарь '%1' уже имеет значение").arg(#pMap));

namespace Qk
{
namespace Core
{

class QKCORE_EXPORT Error
{
public:
    Error();
    Error(quint32 pCode, const QString& pLocation, const QString& pMessage, const QString& pDetails = QString());
    Error(const std::exception& pEx);

    virtual ~Error()
    {
    }

    bool hasError() const { return mCode != 200; }
    QString message() const { return mMessage; }
    QString details() const { return mDetails; }
    QString location() const { return mLocation; }
    QString backtrace() const { return mBacktrace; }
    quint32 code() const { return mCode; }
    QString toString() const { return QString("%2.\n%1\n%3").arg(mLocation).arg(mMessage).arg(mDetails); }

private:
    quint32 mCode = 200;
    QString mMessage;
    QString mLocation;
    QString mDetails;
    QString mBacktrace;
};

class QKCORE_EXPORT ErrorNotImplemented : public Error
{
public:
    ErrorNotImplemented(const QString& pLocation)
        : Error(501, pLocation, qApp->translate("", "Функция не реализована"))
    {
    }
};

class QKCORE_EXPORT ErrorBadArgument : public Error
{
public:
    ErrorBadArgument(const QString& pLocation, const QString& pArgName, const QString& pArgValue, const QString& pCondition)
        : Error(412, pLocation, qApp->translate("", "Параметр '%1', со значением '%2' не отвечает условию %3").arg(pArgName).arg(pArgValue).arg(pCondition))
    {
    }
    ErrorBadArgument(const QString& pLocation, const QString& pMessage)
        : Error(412, pLocation, pMessage)
    {
    }
};

class QKCORE_EXPORT ErrorNotAuthorized : public Error
{
public:
    ErrorNotAuthorized(const QString& pLocation, const QString& pReason)
        : Error(403, pLocation, qApp->translate("", "У вас нет прав доступа на выполнение этой операции."), pReason)
    {
    }
};

class QKCORE_EXPORT ErrorNotAuthenticated : public Error
{
public:
    ErrorNotAuthenticated(const QString& pLocation, const QString& pDetails = "")
        : Error(401, pLocation, qApp->translate("", "Для выполнения этой операции требуется аутентификация"), pDetails)
    {
    }
};

class QKCORE_EXPORT ErrorObjectNotFound : public Error
{
public:
    ErrorObjectNotFound(const QString& pLocation, const QString& pDetails = "")
        : Error(404, pLocation, qApp->translate("", "Запрашиваемый объект не найден"), pDetails)
    {
    }
};

}
}
