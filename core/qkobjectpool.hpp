#pragma once

#include <QList>
#include <QQueue>
#include "qkthrowable.hpp"

class QkPooledObject;

class QkObjectPoolBase : public QkThrowable
{
public:
    virtual ~QkObjectPoolBase()
    {
    }

    virtual void release(QkPooledObject* pObject) = 0;
};

class QkPooledObject
{
    template<class TObject> friend class QkPoolPtr;
    template<class TObject, class TAlloc> friend class QkObjectPool;

public:
    static constexpr bool createNewObjectInPtr = false;

public:
    virtual ~QkPooledObject()
    {
    }

private:
    QkObjectPoolBase* mPool = nullptr;
    QAtomicInt mRefCount;
};

template<class TObject>
class QkPoolPtr
{
public:
    QkPoolPtr(TObject* pObject)
        : mObject(pObject)
    {
        if (mObject) mObject->mRefCount.ref();
    }

    QkPoolPtr(const QkPoolPtr& pCopy)
    {
        mObject = pCopy.mObject;
        if (mObject) mObject->mRefCount.ref();
    }

    ~QkPoolPtr()
    {
        if (mObject && mObject->mRefCount.deref())
        {
            if (mObject->mPool)
            {
                mObject->mPool->release(mObject);
            }
            else
            {
                delete mObject;
            }
        }
    }

public:
    TObject* operator->()
    {
        return mObject;
    }

    const TObject* operator->() const
    {
        return mObject;
    }

private:
    TObject* mObject = nullptr;
};

class QkObjectPoolAllocator
{
public:
    template<class TObject>
    static TObject* create()
    {
        return new TObject();
    }

    virtual QkPooledObject* createVirtual() = 0;
};

template<class TObject, class TAllocator = QkObjectPoolAllocator >
class QkObjectPool : public QkObjectPoolBase
{
public:
    QkObjectPool(quint32 pMaxSize = 10, QkObjectPoolAllocator* pAllocator = nullptr)
        : mAllocator(pAllocator), mMaxSize(pMaxSize)
    {
    }

    virtual ~QkObjectPool()
    {
        qDeleteAll(mFree);
        foreach (QkPooledObject* obj, mBusy)
        {
            obj->mPool = nullptr;
        }
        mBusy.clear();
    }

public:
    QkPoolPtr<TObject> aquire()
    {
        if (mFree.size() > 0)
        {
            QkPooledObject* obj = mFree.dequeue();
            mBusy.enqueue(obj);
            return QkPoolPtr<TObject>((TObject*)obj);
        }
        else if (mFree.size() + mBusy.size() < mMaxSize)
        {
            TObject* obj = createNewObject();
            obj->mPool = this;
            mBusy.enqueue(obj);
            return QkPoolPtr<TObject>(obj);
        }
        else
        {
            throwNow(ERRLOC, "Pool is full");
            return QkPoolPtr<TObject>(nullptr);
        }
    }

    virtual void release(QkPooledObject *pObject) override
    {
        if (pObject)
        {
            mBusy.removeOne(pObject);
            mFree.enqueue(pObject);
        }
    }

private:
    TObject* createNewObject()
    {
        return mAllocator ? (TObject*)mAllocator->createVirtual() : TAllocator::template create<TObject>();
    }

    QQueue<QkPooledObject*> mFree;
    QQueue<QkPooledObject*> mBusy;\
    QkObjectPoolAllocator* mAllocator = nullptr;
    quint32 mMaxSize = 0;
};
