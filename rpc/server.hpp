#pragma once

#include <QMap>
#include <QString>
#include <QThreadPool>
#include <qk/core/log.hpp>
#include "sessionstorage.hpp"
#include "transport.hpp"
#include "context.hpp"
#include "handler.hpp"

using namespace Qk::Core;

namespace Qk {
namespace Rpc {

class SrvLogLevel : public Enum
{
    Q_OBJECT

public:
    enum Value
    {
        None            = 0x00,
        Requests        = 0x01,
        RequestData     = 0x02,
        RequestRawData  = 0x04,
        ResponseData    = 0x08,
        All             = 0xFF
    };
    QKFLAG(Value)
};
QKENUMDECL(SrvLogLevel::Value, ESrvLogLevel)

class Server : public QObject
{
    Q_OBJECT

    friend class Handler;

public:
    Server(SessionStorage* pSessionStorage = 0);
    virtual ~Server();

public:
    void addTransport(Transport* pTransport);
    void removeTransport(Transport* pTransport);

    ESrvLogLevel logLevel() const { return mLogLevel; }
    void logLevel(ESrvLogLevel pLevel) { mLogLevel = pLevel; }

    template<class THandler>
    void addHandler(const QRegExp& pPath) { addHandler(pPath, new THandler(this)); }
    void addHandler(const QRegExp& pPath, Handler* pHandler);
    template<class TThis>
    void addSyncHandler(const QRegExp& pPath, TThis* pThis, HandlerProc<TThis> pProc)
    {
        addHandler(pPath, new HandlerFunctor<TThis>(this, pThis, pProc, false));
    }
    void addSyncHandler(const QRegExp& pPath, const std::function<void(Context*)>& pFunc)
    {
        addHandler(pPath, new HandlerStdFunctor(this, pFunc, false));
    }
    template<class TThis>
    void addAsyncHandler(const QRegExp& pPath, TThis* pThis, HandlerProc<TThis> pProc)
    {
        addHandler(pPath, new HandlerFunctor<TThis>(this, pThis, pProc, true));
    }
    void addAsyncHandler(const QRegExp& pPath, const std::function<void(Context*)>& pFunc)
    {
        addHandler(pPath, new HandlerStdFunctor(this, pFunc, true));
    }
    void removeHandler(Handler* pHandler);

    void init();
    void run();
    void pause();
    void stop();

protected slots:
    void onRequest(Transport* pTransport, Context* pContext) noexcept;

private:
    void runAsyncRequest(QRunnable* pRun);

private:
    QThreadPool mThreads;
    SessionStorage* mSessionStorage;
    QList<QPair<QRegExp, Handler*>> mHandlers;
    QList<Transport*> mTransport;
    bool mInitialized = false;
    quint32 mRequestCounter = 0;
    ESrvLogLevel mLogLevel = SrvLogLevel::Requests;
};

}
}
