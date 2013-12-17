#include "session.hpp"
#include "server.hpp"

namespace Qk {
namespace Rpc {

Session::Session()
    : mID(QUuid::createUuid()), mLog(rpclog(), mID.toString().right(8).left(7))
{
}

Session::~Session()
{
}

void Session::abandon()
{
    log()->info(tr("Сессия прервана. Дата/время последнего запроса: %1").arg(mLastRequestTime.toString()));
}

void Session::requestStart(Context*)
{
    mLocked = false;
}

void Session::requestProcessed(Context*)
{
    mLastRequestTime = QDateTime::currentDateTime();
    mProcessedRequestCount++;
    mLocked = true;
}

}
}
