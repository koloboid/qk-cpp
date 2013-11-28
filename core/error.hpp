#pragma once

#include "core.export.hpp"

#include <QString>
#include <QCoreApplication>

#define ERRLOC qApp->translate("", "Файл: %1:%2, Функция: %3").arg(__FILE__).arg(__LINE__).arg(__PRETTY_FUNCTION__)
#define TR(x) qApp->translate("", x)

#define ASSERTPTR(pName) if (pName == nullptr) throw Qk::Core::Error(ERRLOC, qApp->translate("Ошибка", "Параметр '%1' не может быть NULL").arg(#pName));

#define ASSERTSTR(pStr, pMin, pMax) if (QString(pStr).size() < pMin || QString(pStr).size() > pMax) \
    throw Qk::Core::Error(ERRLOC, qApp->translate("Ошибка", "Параметр '%1', со значением '%2', должен быть длиной от %3 до %4 знаков").arg(#pStr).arg(pStr).arg(pMin).arg(pMax));

#define ASSERTMAPUNIQKEY(pMap, pKey) if (pMap.contains(pKey)) \
    throw Qk::Core::Error(ERRLOC, qApp->translate("Ошибка", "Словарь '%1' уже имеет ключ '%2'").arg(#pMap).arg(pKey));

#define ASSERTMAPUNIQVAL(pMap, pVal) if (pMap.values().contains(pVal)) \
    throw Qk::Core::Error(ERRLOC, qApp->translate("Ошибка", "Словарь '%1' уже имеет значение").arg(#pMap));

namespace Qk
{
namespace Core
{

class QKCORE_EXPORT Error
{
public:
    Error()
    {
    }
    Error(const QString& pLocation, const QString& pMessage, const QString& pDetails = QString());

    virtual ~Error()
    {
    }

    bool hasError() const { return !mMessage.isEmpty() || !mLocation.isEmpty(); }
    QString message() const { return mMessage; }
    QString details() const { return mDetails; }
    QString location() const { return mLocation; }
    QString backtrace() const { return mBacktrace; }
    QString toString() const { return QString("%2.\n%1\n%3").arg(mLocation).arg(mMessage).arg(mDetails); }

private:
    QString mMessage;
    QString mLocation;
    QString mDetails;
    QString mBacktrace;
};

class QKCORE_EXPORT ErrorNotImplemented : public Error
{
public:
    ErrorNotImplemented(const QString& pLocation)
        : Error(pLocation, qApp->translate("", "Функция не реализована"))
    {
    }
};

}
}
