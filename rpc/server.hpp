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

    template<class THandler>
    void addHandler() { addHandler(new THandler(this)); }
    void addHandler(Handler* pHandler);
    template<class TThis>
    void addSyncHandler(const QRegExp& pPath, TThis* pThis, HandlerProc<TThis> pProc)
    {
        HandlerFunctor<TThis>* hdl = new HandlerFunctor<TThis>(this, pPath, pThis, pProc, false);
        mHandlerList.append(hdl);
    }
    void addSyncHandler(const QRegExp& pPath, const std::function<void(Context*)>& pFunc)
    {
        HandlerStdFunctor* hdl = new HandlerStdFunctor(this, pPath, pFunc, false);
        mHandlerList.append(hdl);
    }
    template<class TThis>
    void addAsyncHandler(const QRegExp& pPath, TThis* pThis, HandlerProc<TThis> pProc)
    {
        HandlerFunctor<TThis>* hdl = new HandlerFunctor<TThis>(this, pPath, pThis, pProc, true);
        mHandlerList.append(hdl);
    }
    void addAsyncHandler(const QRegExp& pPath, const std::function<void(Context*)>& pFunc)
    {
        HandlerStdFunctor* hdl = new HandlerStdFunctor(this, pPath, pFunc, true);
        mHandlerList.append(hdl);
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

    QList<Handler*> mHandlerList;
};

}
}
