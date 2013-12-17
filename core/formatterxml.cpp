#include "formatterxml.hpp"
#include "error.hpp"

namespace Qk {
namespace Core {
/*
FormatterXml::FormatterXml(QIODevice* pOutDevice)
    : mOut(pOutDevice)
{
    mOut->write("{");
}

QVariantMap FormatterXml::read()
{
    throw ErrorNotImplemented(ERRLOC);
}

Formatter& FormatterXml::startObject(const QString& pName)
{
    mOut->write("\"");
    mOut->write(pName.toUtf8());
    mOut->write("\"");
    mOut->write(":{");
    return *this;
}

Formatter& FormatterXml::endObject()
{
    mOut->write("}");
    return *this;
}

Formatter& FormatterXml::startArray(const QString& pName)
{
    mOut->write("\"");
    mOut->write(pName.toUtf8());
    mOut->write("\"");
    mOut->write(":[");
    return *this;
}

Formatter& FormatterXml::endArray()
{
    mOut->write("]");
    return *this;
}

Formatter& FormatterXml::write(const QString& pName, const QVariant& pValue)
{
    mOut->write("\"");
    mOut->write(pName.toUtf8());
    mOut->write("\":");
    switch (pValue.type())
    {
        case QVariant::Bool:
        case QVariant::Char:
        case QVariant::Double:
        case QVariant::Int:
        case QVariant::LongLong:
        case QVariant::UInt:
        case QVariant::ULongLong:
            mOut->write(pValue.toString().toUtf8());
            break;
        default:
            mOut->write("\"");
            mOut->write(pValue.toString().toUtf8());
            mOut->write("\"");
            break;
    }
    return *this;
}
*/
}
}
