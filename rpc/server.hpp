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

public:
    Server(SessionStorage* pSessionStorage = 0);
    virtual ~Server();

public:
    void addHandler(Handler* pHandler);
    void removeHandler(Handler* pHandler);
    void addTransport(Transport* pTransport);
    void removeTransport(Transport* pTransport);

    void init();
    void run();
    void pause();
    void stop();

protected slots:
    void onRequest(Transport* pTransport, Context* pContext) noexcept;

private:
    SessionStorage* mSessionStorage;
    QMap<QString, Handler*> mHandlers;
    QList<Transport*> mTransport;
    QThreadPool mThreadPool;
};

Log* rpclog(Log* pParent = 0);

}
}
