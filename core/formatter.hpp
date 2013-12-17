#pragma once

#include <QByteArray>
#include <QVariant>
#include <QString>
#include <QIODevice>

namespace Qk {
namespace Core {

class Formatter
{
public:
    Formatter();
    virtual ~Formatter();

public:
    virtual QString contentType() const = 0;
    virtual QString extension() const = 0;

    virtual QVariantMap read() = 0;

    virtual Formatter& startObject(const QString& pName) = 0;
    virtual Formatter& endObject() = 0;
    virtual Formatter& startArray(const QString& pName) = 0;
    virtual Formatter& endArray() = 0;
    virtual Formatter& write(const QString& pName, const QVariant& pValue) = 0;
    virtual Formatter& write(const QString& pName, const QString& pValue) = 0;
    virtual Formatter& write(const QString& pName, const char* pValue) = 0;
    template<class T>
    Formatter& write(const QString &pName, const T& pValue) { return write(pName, QVariant::fromValue<T>(pValue)); }

    virtual QIODevice* flush() = 0;
};

}
}
