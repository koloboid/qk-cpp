#pragma once

#include <QObject>
#include <QMetaEnum>
#include "error.hpp"

namespace Qk
{
namespace Core
{

class Enum : public QObject
{
    Q_OBJECT
private:
    Enum()
    {
    }
};

#define QKENUM(pName) Q_ENUMS(pName) \
private:\
    static QMetaEnum getMetaEnum() { return staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator(#pName)); } \
public: \
    static QString getName(pName pVal, const QString& pDefault = QString()) { auto t = getMetaEnum().valueToKey(pVal); return t ? t : pDefault; }\
    static QString getTitle(pName pVal, const QString& pDefault = QString()) { return TR(getName(pVal, pDefault).toUtf8().data()); }\
    static pName getValue(const QString& pVal, pName pDefault = pName()) { bool ok; pName t = (pName)getMetaEnum().keyToValue(pVal.toUtf8().data(), &ok); return ok ? t : pDefault; }\
    static pName getValue(const QString& pVal, bool* pOk) { return (pName)getMetaEnum().keyToValue(pVal.toUtf8().data(), pOk); }\
    static QList<pName> values() { QList<pName> list; QMetaEnum e = getMetaEnum(); for (int i = 0; i < e.keyCount(); i++) list << (pName)e.value(i); return list; }

#define QKFLAG(pName) Q_FLAGS(pName) \
private:\
    static QMetaEnum getMetaEnum() { return staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator(#pName)); } \
public: \
    static QString getName(pName pVal, const QString& pDefault = QString()) { auto t = getMetaEnum().valueToKey(pVal); return t ? t : pDefault; }\
    static QString getTitle(pName pVal, const QString& pDefault = QString()) { return TR(getName(pVal, pDefault).toUtf8().data()); }\
    static pName getValue(const QString& pVal, pName pDefault = pName()) { bool ok; pName t = (pName)getMetaEnum().keyToValue(pVal.toUtf8().data(), &ok); return ok ? t : pDefault; }\
    static pName getValue(const QString& pVal, bool* pOk) { return (pName)getMetaEnum().keyToValue(pVal.toUtf8().data(), pOk); }\
    static QList<pName> values() { QList<pName> list; QMetaEnum e = getMetaEnum(); for (int i = 0; i < e.keyCount(); i++) list << (pName)e.value(i); return list; }

#define QKENUMDECL(pTypeName, pNewType) \
    typedef pTypeName pNewType; \
    inline pTypeName operator|(pTypeName p1, pTypeName p2) { return (pTypeName)((int)p1 | (int)p2); }



}
}
