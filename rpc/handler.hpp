#pragma once

#include <QObject>
#include <qk/core/log.hpp>

namespace Qk {
namespace Rpc {

class Context;
class Server;

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
    virtual void processRequest(Context* pCtx, bool pIsServerThread);
    virtual void methodNotFound(const QString& pMethod, Context* pCtx, bool pIsServerThread);

private:
    void processRequestInternal(Context* pCtx);
    void callMethod(int pMethodIndex, Context* pCtx);

private:
    Server* mServer = nullptr;
    QMap<QString, int> mSyncMethods;
    QMap<QString, int> mAsyncMethods;
};

}
}

#ifndef Q_MOC_RUN

#define QKRPC
#define QKRPCASYNC

#endif
