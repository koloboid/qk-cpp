#include "formatterjson.hpp"
#include "error.hpp"

namespace Qk {
namespace Core {

FormatterJson::FormatterJson(QIODevice* pOutDevice)
    : mOut(pOutDevice)
{
    mOut->write("{");
}

QVariantMap FormatterJson::read()
{
    throw ErrorNotImplemented(ERRLOC);
}

Formatter& FormatterJson::startObject(const QString& pName)
{
    mOut->write("\"");
    mOut->write(pName.toUtf8());
    mOut->write("\"");
    mOut->write(":{");
    return *this;
}

Formatter& FormatterJson::endObject()
{
    mOut->write("}");
    return *this;
}

Formatter& FormatterJson::startArray(const QString& pName)
{
    mOut->write("\"");
    mOut->write(pName.toUtf8());
    mOut->write("\"");
    mOut->write(":[");
    return *this;
}

Formatter& FormatterJson::endArray()
{
    mOut->write("]");
    return *this;
}

Formatter& FormatterJson::write(const QString& pName, const QVariant& pValue)
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

}
}
