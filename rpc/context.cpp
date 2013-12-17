#include <qk/core/formatterjson.hpp>

#include "context.hpp"
#include "session.hpp"

namespace Qk {
namespace Rpc {

Context::Context()
{

}

Context::~Context()
{
    if (mOut) delete mOut;
}

void Context::respondError(const QString& pError, quint32 pStatusCode)
{
    mStatusCode = pStatusCode;
    mRespondErrors.append(pError);
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

void Context::finish()
{
    if (mRespondErrors.size() > 0)
    {
        out()->startArray("errorList");
        foreach (const QString& err, mRespondErrors)
        {
            out()->write("", err);
        }
    }
    out()->flush();
}

void Context::start()
{
    // main server thread

    mResponseBuffer.open(QIODevice::WriteOnly);
    if (path().endsWith(".xml"))
    {
        //mOut = new FormatterXml(&mResponseBuffer);
    }
    else
    {
        mOut = new FormatterJson(&mResponseBuffer);
    }
}

}
}
