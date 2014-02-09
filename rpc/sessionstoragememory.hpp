#pragma once

#include <QMap>
#include <qk/core/timespan.hpp>
#include "sessionstorage.hpp"

using namespace Qk::Core;

namespace Qk {
namespace Rpc {

class SessionStorageMemory : public SessionStorage
{
    Q_OBJECT

public:
    SessionStorageMemory(const TimeSpan& pSessionLifeTime, quint32 pMaxSessionsCount = 100000);
    virtual ~SessionStorageMemory();

public:
    virtual Session* getSession(QUuid pID) override;

private:
    quint32 mMaxSessionsCount;
};

}
}
