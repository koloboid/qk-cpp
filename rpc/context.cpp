#include "context.hpp"
#include "session.hpp"

namespace Qk {
namespace Rpc {

Context::Context()
{

}

void Context::respondError(const QString& pError, quint32 pStatusCode)
{
    mStatusCode = pStatusCode;
    mRepondErrors.append(pError);
}

void Context::setSession(Session* pSession)
{
    ASSERTPTR(pSession);
    mSession = pSession;
}

QString Context::statusText() const
{
    if (mStatusCode >= 500)
    {
        return "Server Error";
    }
    else if (mStatusCode >= 400)
    {
        return "Client error";
    }
    else if (mStatusCode >= 300)
    {
        return "Redirection";
    }
    else if (mStatusCode >= 200)
    {
        return "Success";
    }
    else if (mStatusCode >= 100)
    {
        return "Information";
    }
    return "Unknown";
}

}
}
