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
    RpcRunnable(Handler* pHandler, Context* pCtx, int pMethodIndex = -1)
        : mHandler(pHandler), mContext(pCtx), mMethodIndex(pMethodIndex)
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
            if (mMethodIndex >= 0)
            {
                mHandler->callMethod(mMethodIndex, mContext);
            }
            else
            {
                mHandler->processRequest(mContext, false);
            }
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
    int mMethodIndex = -1;
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
            mSyncMethods[name] = i;
        }
        else if (tag == "QKRPCASYNC")
        {
            mAsyncMethods[name] = i;
        }
    }
}

void Handler::newSession(Context*)
{
}

void Handler::processRequest(Context* pCtx, bool pIsServerThread)
{
    QFileInfo file(pCtx->path());
    QString methodname = file.fileName();
    methodname = methodname.left(methodname.indexOf('.'));
    int mtidx = mSyncMethods.value(methodname, -1);
    if (mtidx >= 0)
    {
        callMethod(mtidx, pCtx);
    }
    else
    {
        mtidx = mAsyncMethods.value(methodname, -1);
        if (mtidx >= 0)
        {
            if (pIsServerThread)
            {
                RpcRunnable* run = new RpcRunnable(this, pCtx, mtidx);
                mServer->runAsyncRequest(run);
            }
            else
            {
                callMethod(mtidx, pCtx);
            }
        }
        else
        {
            methodNotFound(methodname, pCtx, pIsServerThread);
        }
    }
}

void Handler::methodNotFound(const QString& pMethodName, Context* pCtx, bool)
{
    pCtx->log()->warn(tr("Метод '%1' не найден в обработчике пути '%2'").arg(pMethodName).arg(pCtx->path()));
}

void Handler::processRequestInternal(Context* pCtx)
{
    if (isAsync(pCtx))
    {
        RpcRunnable* run = new RpcRunnable(this, pCtx);
        mServer->runAsyncRequest(run);
    }
    else
    {
        processRequest(pCtx, true);
        pCtx->finish();
    }
}

void Handler::callMethod(int pMethodIndex, Context* pCtx)
{
    QMetaMethod mt = this->metaObject()->method(pMethodIndex);
    if (!mt.invoke(this, Qt::DirectConnection, Q_ARG(Context*, pCtx)))
    {
        throw Error(ERRLOC, tr("Метод '%1' не вызван по причине несовпадающих параметров").arg(QString(mt.name())));
    }
}

}
}
