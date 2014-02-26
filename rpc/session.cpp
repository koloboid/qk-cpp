#include "session.hpp"
#include "server.hpp"

namespace Qk {
namespace Rpc {

Session::Session()
    : mID(QUuid::createUuid())
{
}

Session::~Session()
{
}

void Session::abandon()
{
    rpclog()->info(tr("Сессия '%2' устарела. Дата/время последнего запроса: %1")
                   .arg(mLastRequestTime.toString())
                   .arg(mID.toString()));
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
