#include <qk/core/log.hpp>
#include "sessionstoragememory.hpp"
#include "server.hpp"
#include "handler.hpp"

using namespace Qk::Core;

namespace Qk {
namespace Rpc {

Log* rpclog(Log* pParent)
{
    static Log* l = new Log(pParent ? pParent : Qk::Core::log(), "RPC");
    return l;
}

class RpcRunnable : public QRunnable
{
public:
    RpcRunnable(Handler* pHandler, Context* pCtx)
        : mHandler(pHandler), mContext(pCtx)
    {
        ASSERTPTR(mHandler);
        ASSERTPTR(mContext);
    }

public:
    virtual void run() override
    {
        // handler thread
        try
        {
            mHandler->processRequest(mContext);
        }
        catch (const Error& pErr)
        {
            rpclog()->error(pErr);
        }
        catch (const std::exception& pStdErr)
        {
            rpclog()->error(pStdErr);
        }
        mContext->finish();
    }

private:
    Handler* mHandler;
    Context* mContext;
};

Server::Server(SessionStorage* pSessionStorage)
    : mSessionStorage(pSessionStorage ? pSessionStorage : new SessionStorageMemory(60000))
{
}

Server::~Server()
{
    stop();
    qDeleteAll(mTransport);
    qDeleteAll(mHandlers);
    delete mSessionStorage;
}

void Server::addHandler(Handler* pHandler)
{
    ASSERTPTR(pHandler);

    rpclog()->debug(tr("Добавлен обработчик пути '%1'").arg(pHandler->path()));
    mHandlers[pHandler->path()] = pHandler;
    pHandler->setParent(this);
}

void Server::removeHandler(Handler* pHandler)
{
    ASSERTPTR(pHandler);

    mHandlers.remove(pHandler->path());
    pHandler->setParent(nullptr);
}

void Server::addTransport(Transport* pTransport)
{
    ASSERTPTR(pTransport);

    mTransport.append(pTransport);
    connect(pTransport, &Transport::onRequest, this, &Server::onRequest);
    pTransport->setParent(this);
}

void Server::removeTransport(Transport* pTransport)
{
    ASSERTPTR(pTransport);

    mTransport.removeOne(pTransport);
    disconnect(pTransport, 0, 0, 0);
    pTransport->setParent(nullptr);
}

void Server::init()
{
    foreach (Handler* hdl, mHandlers)
    {
        hdl->init();
    }
    foreach (Transport* trans, mTransport)
    {
        trans->init();
    }
}

void Server::run()
{
    foreach (Transport* trans, mTransport)
    {
        trans->run();
    }
}

void Server::pause()
{
    foreach (Transport* trans, mTransport)
    {
        trans->pause();
    }
}

void Server::stop()
{
    foreach (Transport* trans, mTransport)
    {
        trans->stop();
    }
}

void Server::onRequest(Transport*, Context* pContext) noexcept
{
    // main server thread
    try
    {
        Session* session = mSessionStorage->getSession(pContext->sessionId());
        pContext->setSession(session);
        session->requestStart(pContext);
        if (session->isNew())
        {
            foreach (Handler* hdl, mHandlers)
            {
                hdl->newSession(pContext);
            }
        }
        QString path = pContext->path();
        Handler* hdl = mHandlers.value(path);
        if (hdl)
        {
            mThreadPool.start(new RpcRunnable(hdl, pContext));
            return;
        }
        else
        {
            session->log()->warn(tr("Не определен обработчик для пути '%1'").arg(path));
            pContext->respondError(tr("Страница не найдена"), 404);
        }
    }
    catch (const Error& pErr)
    {
        rpclog()->error(pErr);
        pContext->respondError(pErr);
    }
    catch (const std::exception& pStdErr)
    {
        rpclog()->error(pStdErr);
        pContext->respondError(pStdErr.what());
    }
    pContext->finish();
}

}
}
