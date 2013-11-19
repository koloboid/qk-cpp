#pragma once

#include "core.export.hpp"
#include "valref.hpp"

#include <QByteArray>
#include <QMetaType>

namespace Qk
{
namespace Core
{

class QKCORE_EXPORT Blob : public QByteArray
{
public:
    Blob()
    {
    }

    Blob(const QByteArray& pArr)
        : QByteArray(pArr)
    {
    }

    Blob(quint8* pData, uint pSize)
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
    ValRef<QByteArray, quint8> operator[](int i) { return ValRef<QByteArray, quint8>(*this, i); }
    ValRef<QByteArray, quint8> operator[](uint i) { return ValRef<QByteArray, quint8>(*this, i); }
};

}
}

Q_DECLARE_METATYPE(Qk::Core::Blob)
