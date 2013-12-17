#include "formatterjson.hpp"
#include "error.hpp"

namespace Qk {
namespace Core {

FormatterJson::FormatterJson(QIODevice* pOutDevice)
    : mOut(pOutDevice)
{
    ASSERTPTR(mOut);
    mOut->write("{");
}

QVariantMap FormatterJson::read()
{
    throw ErrorNotImplemented(ERRLOC);
}

Formatter& FormatterJson::startObject(const QString& pName)
{
    writeHead(pName);
    mOut->write("{");
    mState = Start;
    mToFinishBrace += "}";
    return *this;
}

Formatter& FormatterJson::endObject()
{
    if (mToFinishBrace.size() > 0 && mToFinishBrace.endsWith("}"))
    {
        mOut->write("}");
        mToFinishBrace = mToFinishBrace.left(mToFinishBrace.size() - 1);
    }
    return *this;
}

Formatter& FormatterJson::startArray(const QString& pName)
{
    writeHead(pName);
    mOut->write("[");
    mState = Start;
    mToFinishBrace += "]";
    return *this;
}

Formatter& FormatterJson::endArray()
{
    if (mToFinishBrace.size() > 0 && mToFinishBrace.endsWith("]"))
    {
        mOut->write("]");
        mToFinishBrace = mToFinishBrace.left(mToFinishBrace.size() - 1);
    }
    return *this;
}

Formatter& FormatterJson::write(const QString& pName, const QString& pValue)
{
    writeHead(pName);
    mOut->write("\"");
    QString val = pValue;
    mOut->write(val.replace("\n", "\\n").toUtf8());
    mOut->write("\"");
    return *this;
}

Formatter& FormatterJson::write(const QString& pName, const char* pValue)
{
    ASSERTPTR(pValue);

    writeHead(pName);
    mOut->write("\"");
    mOut->write(QString(pValue).replace("\n", "\\n").toUtf8());
    mOut->write("\"");
    return *this;
}

Formatter& FormatterJson::write(const QString& pName, const QVariant& pValue)
{
    writeHead(pName);
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
            mOut->write(pValue.toString().replace("\n", "\\n").toUtf8());
            mOut->write("\"");
            break;
    }
    return *this;
}

QIODevice* FormatterJson::flush()
{
    mOut->write(mToFinishBrace.toUtf8());
    mOut->write("}");
    return mOut;
}

void FormatterJson::writeHead(const QString& pName)
{
    if (mState == NeedComma) mOut->write(",");
    if (mToFinishBrace.size() == 0 || mToFinishBrace.endsWith("}"))
    {
        mOut->write("\"");
        mOut->write(pName.toUtf8());
        mOut->write("\":");
    }
    mState = NeedComma;
}

}
}
