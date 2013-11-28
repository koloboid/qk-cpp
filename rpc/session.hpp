#pragma once

#include <QUuid>
#include <QDateTime>
#include <QObject>
#include <qk/core/log.hpp>

using namespace Qk::Core;

namespace Qk {
namespace Rpc {

class Context;

class Session : public QObject
{
    Q_OBJECT

    friend class Server;

public:
    Session();
    virtual ~Session();

public:
    QUuid id() const { return mID; }
    bool isNew() const { return mProcessedRequestCount == 0; }
    QDateTime lastRequestTime() const { return mLastRequestTime; }
    bool isLocked() const { return mLocked; }
    Log* log() { return &mLog; }
    void abandon();

    void requestStart(Context* pCtx);
    void requestProcessed(Context* pCtx);

private:
    QUuid mID;
    quint32 mProcessedRequestCount = 0;
    QDateTime mLastRequestTime;
    bool mLocked = false;
    Log mLog;
};

}
}
