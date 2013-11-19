#include "logitem.hpp"
#include "log.hpp"

namespace Qk
{
namespace Core
{

LogItem& LogItem::backtrace(const QString& pBacktrace)
{
    mData->mStack = pBacktrace.isEmpty() ? LogBase::getBacktrace() : pBacktrace;
    return *this;
}

LogItem::Data::~Data()
{
    if (mLog)
    {
        LogItem copyitem(new LogItem::Data(*this));
        copyitem.mData->mLog = 0;
        if (mDirect)
        {
            copyitem.mData->mFlushMutex = new QMutex();
            copyitem.mData->mFlushCond = new QWaitCondition();
            mLog->direct(copyitem);
        }
        else mLog->enqueue(copyitem);
        mLog = 0;
    }
    if (mFlushMutex) delete mFlushMutex;
    if (mFlushCond) delete mFlushCond;
}

void LogItem::direct()
{
    mData->mDirect = true;
}

void LogItem::waitFlush()
{
    QMutexLocker lock(mData->mFlushMutex);
    if (!mData->mFlushed)
    {
        mData->mFlushCond->wait(mData->mFlushMutex);
    }
}

void LogItem::flushed()
{
    if (mData->mDirect && !mData->mFlushed && mData->mFlushMutex)
    {
        QMutexLocker lock(mData->mFlushMutex);
        mData->mFlushCond->wakeOne();
        mData->mFlushed = true;
    }
}

}
}
