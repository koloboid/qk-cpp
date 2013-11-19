#pragma once

#include "timespan.hpp"

#include <QMutex>
#include <QQueue>
#include <QMutexLocker>
#include <QWaitCondition>

namespace Qk
{
namespace Core
{

template<class T>
class BlockQueue
{
public:
    void enqueue(const T& pVal)
    {
        QMutexLocker lock(&mMutex);
        mQueue.enqueue(pVal);
        mCond.wakeOne();
    }

    T dequeue(TimeSpan pTime = TimeSpan())
    {
        QMutexLocker lock(&mMutex);
        if (mQueue.size() > 0) return mQueue.dequeue();
        mCond.wait(&mMutex, pTime.totalMsec());
        if (mQueue.size() > 0) return mQueue.dequeue();
        else return T();
    }

private:
    QQueue<T> mQueue;
    QMutex mMutex;
    QWaitCondition mCond;
};

}
}
