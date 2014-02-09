#include "sessionstorage.hpp"
#include "server.hpp"

namespace Qk {
namespace Rpc {

SessionStorage::SessionStorage(const TimeSpan& pSessionLifeTime)
    : mLifeTime(pSessionLifeTime)
{
    rpclog()->debug(ERRLOC).arg("TimerStart result", startTimer(1000));
}

SessionStorage::~SessionStorage()
{
    qDeleteAll(mSessions);
}

void SessionStorage::timerEvent(QTimerEvent*)
{
    QDateTime now = QDateTime::currentDateTime();
    for (auto it = mSessions.begin(); it != mSessions.end();)
    {
        if (it.value()->lastRequestTime().msecsTo(now) > mLifeTime.totalMsec())
        {
            it.value()->abandon();
            delete it.value();
            it = mSessions.erase(it);
        }
        else ++it;
    }
}

}
}
