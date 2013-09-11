#include "qklogitem.hpp"
#include "qklog.hpp"

QkLogItem& QkLogItem::appendStack()
{
    mData->mStack = QkLogBase::getBacktrace();
    return *this;
}

QkLogItem::Data::~Data()
{
    if (mLog)
    {
        QkLogItem copyitem(new QkLogItem::Data(*this));
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

void QkLogItem::waitFlush()
{
    QMutexLocker lock(mData->mFlushMutex);
    if (!mData->mFlushed)
    {
        mData->mFlushCond->wait(mData->mFlushMutex);
    }
}

void QkLogItem::flushed()
{
    if (mData->mDirect && !mData->mFlushed && mData->mFlushMutex)
    {
        QMutexLocker lock(mData->mFlushMutex);
        mData->mFlushCond->wakeOne();
        mData->mFlushed = true;
    }
}
