#pragma once

#include <QObject>
#include <qk/core/log.hpp>

namespace Qk {
namespace Rpc {

class Context;
class Server;

template<class TThis>
using HandlerProc = void(TThis::*)(Context* pCtx);

class Handler : public QObject
{
    Q_OBJECT

    friend class RpcRunnable;
    friend class Server;

public:
    Handler(Server* pParent);

public:
    Server* server() const { return mServer; }
    virtual QRegExp path() = 0;
    virtual void init();
    virtual void newSession(Context* pCtx);

protected:
    virtual bool isAsync(Context*) const { return false; }
    virtual void processRequest(Context* pCtx);
    virtual void methodNotFound(Context* pCtx, const QString& pMethodName);
    virtual void doAsync(Context* pCtx, const std::function<void(Context*)>& pFunc);

private:
    void processRequestInternal(Context* pCtx);
    void callMethod(Context* pCtx, int pMethodIndex);

private:
    Server* mServer = nullptr;
    QMap<QString, int> mMethods;
};

template<class TThis>
class HandlerFunctor : public Handler
{
public:
    HandlerFunctor(Server* pServer, const QRegExp& pPath, TThis* pThis, HandlerProc<TThis> pProc, bool pIsAsync)
        : Handler(pServer), mPath(pPath), mProc(pProc), mThis(pThis), mIsAsync(pIsAsync)
    {
    }

protected:
    virtual QRegExp path() { return mPath; }
    virtual bool isAsync(Context*) const { return mIsAsync; }
    virtual void processRequest(Context* pCtx) override
    {
        (mThis->*mProc)(pCtx);
    }

private:
    QRegExp mPath;
    HandlerProc<TThis> mProc;
    TThis* mThis;
    bool mIsAsync;
};

class HandlerStdFunctor : public Handler
{
public:
    HandlerStdFunctor(Server* pServer, const QRegExp& pPath, const std::function<void(Context*)>& pFunc, bool pIsAsync)
        : Handler(pServer), mPath(pPath), mIsAsync(pIsAsync), mFunc(pFunc)
    {
    }

protected:
    virtual QRegExp path() { return mPath; }
    virtual bool isAsync(Context*) const { return mIsAsync; }
    virtual void processRequest(Context* pCtx) override
    {
        mFunc(pCtx);
    }

private:
    QRegExp mPath;
    bool mIsAsync;
    std::function<void(Context*)> mFunc;
};

}
}

#ifndef Q_MOC_RUN

#define QKRPC
#define QKRPCASYNC

#endif
