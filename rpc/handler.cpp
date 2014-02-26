#include <QRunnable>
#include <QMetaMethod>
#include <QFileInfo>
#include "handler.hpp"
#include "server.hpp"

namespace Qk {
namespace Rpc {

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
            mContext->respondError(pErr);
            rpclog()->error(pErr);
        }
        catch (const std::exception& pStdErr)
        {
            mContext->respondError(pStdErr);
            rpclog()->error(pStdErr);
        }
        mContext->finish();
    }

private:
    Handler* mHandler = nullptr;
    Context* mContext = nullptr;
};

class RpcFuncRunnable : public QRunnable
{
public:
    RpcFuncRunnable(const std::function<void(Context*)>& pFunc, Context* pCtx)
        : mFunc(pFunc), mContext(pCtx)
    {
        ASSERTPTR(mFunc);
        ASSERTPTR(mContext);
    }

public:
    virtual void run() override
    {
        // handler thread
        try
        {
            mFunc(mContext);
        }
        catch (const Error& pErr)
        {
            mContext->respondError(pErr);
            rpclog()->error(pErr);
        }
        catch (const std::exception& pStdErr)
        {
            mContext->respondError(pStdErr);
            rpclog()->error(pStdErr);
        }
        mContext->finish();
    }

private:
    const std::function<void(Context*)>& mFunc;
    Context* mContext = nullptr;
};

Handler::Handler(Server* pParent)
    : QObject(pParent), mServer(pParent)
{
    ASSERTPTR(pParent);
}

void Handler::init()
{
    for (int i = 0; i < this->metaObject()->methodCount(); i++)
    {
        QMetaMethod mt = this->metaObject()->method(i);
        QString name = mt.name();
        QString tag = mt.tag();
        if (tag == "QKRPC")
        {
            mMethods[name] = i;
        }
    }
}

void Handler::newSession(Context*)
{
}

void Handler::processRequest(Context* pCtx)
{
    QFileInfo file(pCtx->path());
    QString methodname = file.fileName();
    methodname = methodname.left(methodname.indexOf('.'));
    int mtidx = mMethods.value(methodname, -1);
    if (mtidx >= 0)
    {
        callMethod(pCtx, mtidx);
    }
    else
    {
        methodNotFound(pCtx, methodname);
    }
}

void Handler::methodNotFound(Context* pCtx, const QString& pMethodName)
{
    pCtx->log()->warn(tr("Метод '%1' не найден в обработчике пути '%2'").arg(pMethodName).arg(pCtx->path()));
}

void Handler::doAsync(Context* pCtx, const std::function<void(Context*)>& pFunc)
{
    if (!pCtx->isServerThread()) pFunc(pCtx);
    else
    {
        RpcFuncRunnable* run = new RpcFuncRunnable(pFunc, pCtx);
        pCtx->hasAsyncCall(true);
        mServer->runAsyncRequest(run);
    }
}

void Handler::processRequestInternal(Context* pCtx)
{
    pCtx->handlerStart(this);
    if (isAsync(pCtx))
    {
        pCtx->hasAsyncCall(true);
        RpcRunnable* run = new RpcRunnable(this, pCtx);
        mServer->runAsyncRequest(run);
    }
    else
    {
        processRequest(pCtx);
    }
}

void Handler::callMethod(Context* pCtx, int pMethodIndex)
{
    QMetaMethod mt = this->metaObject()->method(pMethodIndex);
    if (!mt.invoke(this, Qt::DirectConnection, Q_ARG(Context*, pCtx)))
    {
        throw Error(ERRLOC, tr("Метод '%1' не вызван по причине несовпадающих параметров").arg(QString(mt.name())));
    }
}

}
}
