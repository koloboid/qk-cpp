#pragma once

#include <QObject>

namespace Qk {
namespace Rpc {

class Context;
class Server;

class Transport : public QObject
{
    Q_OBJECT

public:
    Transport(Server* pServer);
    virtual ~Transport();

public:
    virtual void init();
    virtual void run() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    Server* server() const { return mRpcServer; }

signals:
    void onRequest(Transport* pFrom, Context* pCtx);

private:
    Server* mRpcServer;
};

}
}
