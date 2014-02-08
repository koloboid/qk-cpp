#include <qk/core/log.hpp>
#include <QElapsedTimer>
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

Server::Server(SessionStorage* pSessionStorage)
    : mSessionStorage(pSessionStorage ? pSessionStorage : new SessionStorageMemory(60000))
{
}

Server::~Server()
{
    stop();
    qDeleteAll(mTransport);
    for (int i = 0; i < mHandlers.size(); i++)
    {
        delete mHandlers[i].second;
    }
    mHandlers.clear();
    delete mSessionStorage;
}

void Server::addHandler(Handler* pHandler)
{
    ASSERTPTR(pHandler);

    rpclog()->debug(tr("Добавлен обработчик пути '%1'").arg(pHandler->path().pattern()));
    mHandlers.append({ pHandler->path(), pHandler });
    pHandler->setParent(this);
}

void Server::removeHandler(Handler* pHandler)
{
    ASSERTPTR(pHandler);

    for (int i = 0; i < mHandlers.size(); i++)
    {
        if (mHandlers[i].second == pHandler)
        {
            mHandlers.removeAt(i--);
        }
    }
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
    foreach (auto hdl, mHandlers)
    {
        hdl.second->init();
    }
    foreach (Transport* trans, mTransport)
    {
        trans->init();
    }
    mInitialized = true;
}

void Server::run()
{
    if (!mInitialized) init();
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
        pContext->start();
        Session* session = mSessionStorage->getSession(pContext->sessionId());
        pContext->setSession(session);
        session->requestStart(pContext);
        if (session->isNew())
        {
            foreach (auto hdl, mHandlers)
            {
                hdl.second->newSession(pContext);
            }
        }
        QString path = pContext->path();
        Handler* hdl = nullptr;
        for (int i = 0; i < mHandlers.size(); i++)
        {
            if (mHandlers[i].first.exactMatch(path))
            {
                hdl = mHandlers[i].second;
                break;
            }
        }
        if (hdl)
        {
            QElapsedTimer tmr;
            tmr.start();
            hdl->processRequestInternal(pContext);
            if (tmr.elapsed() > 1000)
            {
                pContext->log()->warn(tr("Обработчик '%1' занял %2мс времени серверного потока. Произошла блокировка работы сервера. Переведите обработчик в асинхронный режим")
                                      .arg(path).arg(tmr.elapsed()));
            }
        }
        else
        {
            pContext->log()->warn(tr("Не определен обработчик для пути '%1'").arg(path));
            pContext->respondError(tr("Страница не найдена"), 404);
        }
    }
    catch (const Error& pErr)
    {
        pContext->log()->error(pErr);
        pContext->respondError(pErr);
    }
    catch (const std::exception& pStdErr)
    {
        pContext->log()->error(pStdErr);
        pContext->respondError(pStdErr.what());
    }
    if (!pContext->hasAsyncCall()) pContext->finish();
}

void Server::runAsyncRequest(QRunnable* pRun)
{
    mThreads.start(pRun);
}

}
}