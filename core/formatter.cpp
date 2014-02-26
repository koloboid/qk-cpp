#include "formatter.hpp"
#include <QBuffer>

namespace Qk {
namespace Core {

Formatter::Formatter(QIODevice *pOut)
    : mOut(pOut)
{
    if (!mOut)
    {
        mOut = new QBuffer();
        mOut->open(QIODevice::ReadWrite);
        mIsOwnDevice = true;
    }
}

Formatter::~Formatter()
{
    if (mIsOwnDevice && mOut) delete mOut;
}

QByteArray Formatter::getResult()
{
    flush();
    if (mOut->isReadable() && !mOut->isSequential())
    {
        qint64 pos = mOut->pos();
        mOut->seek(0);
        QByteArray rv = mOut->readAll();
        mOut->seek(pos);
        return rv;
    }
    return QByteArray();
}

}
}
