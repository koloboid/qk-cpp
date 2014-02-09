#include "sessionstoragememory.hpp"

namespace Qk {
namespace Rpc {

SessionStorageMemory::SessionStorageMemory(const TimeSpan& pSessionLifeTime, quint32 pMaxSessionsCount)
    : SessionStorage(pSessionLifeTime),
      mMaxSessionsCount(pMaxSessionsCount)
{
}

SessionStorageMemory::~SessionStorageMemory()
{

}

Session* SessionStorageMemory::getSession(QUuid pID)
{
    Session* s = mSessions.value(pID);
    if (!s || pID.isNull())
    {
        if ((quint32)mSessions.size() > mMaxSessionsCount)
        {
            throw Error(ERRLOC, tr("Максимальное количество сессий достигло предела (%1)").arg(mMaxSessionsCount));
        }
        s = new Session();
        mSessions[s->id()] = s;
    }
    return s;
}

}
}
