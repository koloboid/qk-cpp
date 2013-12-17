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
    template<class THandler>
    void addHandler() { addHandler(new THandler(this)); }
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
    void runAsyncRequest(QRunnable* pRun);

private:
    QThreadPool mThreads;
    SessionStorage* mSessionStorage;
    QList<QPair<QRegExp, Handler*>> mHandlers;
    QList<Transport*> mTransport;
    bool mInitialized = false;
};

}
}
