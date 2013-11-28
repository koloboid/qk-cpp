#pragma once

#include "session.hpp"

namespace Qk {
namespace Rpc {

class SessionStorage : public QObject
{
    Q_OBJECT

public:
    SessionStorage(const TimeSpan& pSessionLifeTime);
    virtual ~SessionStorage();

public:
    virtual Session* getSession(QUuid pID) = 0;

protected:
    virtual void timerEvent(QTimerEvent*) override;

protected:
    TimeSpan mLifeTime;
    QMap<QUuid, Session*> mSessions;
};

}
}
