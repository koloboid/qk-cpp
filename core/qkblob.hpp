#pragma once

#include "qkcore.export.hpp"
#include "qkvalref.hpp"

#include <QByteArray>
#include <QMetaType>

class QKCORE_EXPORT QkBlob : public QByteArray
{
public:
    QkBlob()
    {
    }

    QkBlob(const QByteArray& pArr)
        : QByteArray(pArr)
    {
    }

    QkBlob(quint8* pData, uint pSize)
        : QByteArray((char*)pData, pSize)
    {
    }

    const quint8* data() const { return (const quint8*)QByteArray::data(); }

#ifdef Q_COMPILER_MANGLES_RETURN_TYPE
    const quint8 at(int i) const { return (quint8)QByteArray::at(i); }
    const quint8 operator[](int i) const { return (quint8)QByteArray::operator[](i); }
    const quint8 operator[](uint i) const { return (quint8)QByteArray::operator[](i); }
#else
    quint8 at(int i) const { return (quint8)QByteArray::at(i); }
    quint8 operator[](int i) const { return (quint8)QByteArray::operator[](i); }
    quint8 operator[](uint i) const { return (quint8)QByteArray::operator[](i); }
#endif
    QkValRef<QByteArray, quint8> operator[](int i) { return QkValRef<QByteArray, quint8>(*this, i); }
    QkValRef<QByteArray, quint8> operator[](uint i) { return QkValRef<QByteArray, quint8>(*this, i); }
};

Q_DECLARE_METATYPE(QkBlob)
