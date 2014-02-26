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
    Formatter(QIODevice* pOut);
    virtual ~Formatter();

public:
    virtual QString contentType() const = 0;
    virtual QString extension() const = 0;
    QByteArray getResult();

    virtual Formatter& startObject(const QString& pName) = 0;
    virtual Formatter& endObject() = 0;
    virtual Formatter& startArray(const QString& pName) = 0;
    virtual Formatter& endArray() = 0;
    virtual Formatter& write(const QString& pName, const QVariant& pValue) = 0;
    virtual Formatter& write(const QString& pName, const QString& pValue) = 0;
    virtual Formatter& write(const QString& pName, const char* pValue) = 0;
    template<class T>
    Formatter& write(const QString &pName, const T& pValue) { return write(pName, QVariant::fromValue<T>(pValue)); }
    template<class T>
    Formatter& writeMap(const QString& pName, const QMap<QString, T>& pValueMap);
    template<class T>
    Formatter& writeMap(const QMap<QString, T>& pValueMap);

    virtual QIODevice* flush() = 0;

protected:
    QIODevice* mOut = nullptr;
    bool mIsOwnDevice = false;
};

template<class T>
Formatter& Formatter::writeMap(const QString& pName, const QMap<QString, T>& pValueMap)
{
    this->startObject(pName);
    this->writeMap(pValueMap);
    return this->endObject();
}

template<class T>
Formatter& Formatter::writeMap(const QMap<QString, T>& pValueMap)
{
    foreach (QString key, pValueMap.keys())
    {
        this->write(key, QVariant::fromValue<T>(pValueMap.value(key)));
    }
    return *this;
}

}
}
