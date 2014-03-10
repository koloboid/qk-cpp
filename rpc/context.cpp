#include <qk/core/formatterjson.hpp>

#include "context.hpp"
#include "session.hpp"
#include "server.hpp"

namespace Qk {
namespace Rpc {

Context::Context(Server* pServer)
    : mServer(pServer), mLog(rpclog(), "RPC")
{
    mTimer.start();
}

Context::~Context()
{
    if (mOut) delete mOut;
}

bool Context::isServerThread()
{
    return QThread::currentThread() == mServer->thread();
}

void Context::doAsync(const std::function<void (Context*)>& pFunc)
{
    if (mHandler)
    {
        mHandler->doAsync(this, pFunc);
    }
    else
    {
        throw Error(500, ERRLOC, TR("Невозможно выполнить асинхронный переход, т.к. обработчик запроса не определен"));
    }
}

void Context::die(const Error& pError)
{
    mError = pError;
    log()->error(pError);
    finish();
}

/*
void Context::respondError(const QString& pError, quint32 pStatusCode)
{
    log()->error(pError);
    mStatusCode = pStatusCode;
    mRespondErrors.append(pError);
}
*/
void Context::setSession(Session* pSession)
{
    ASSERTPTR(pSession);
    mSession = pSession;
    updateLogHeader();
}

void Context::updateLogHeader()
{
    if (session())
    {
        mLog.prepend(QString("RPC[%1][%2] ").arg(requestID()).arg(session()->id().toString().right(8).left(7)));
    }
    else
    {
        mLog.prepend(QString("RPC[%1] ").arg(requestID()));
    }
}
/*
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
*/

void Context::finish()
{
    if (mFinished) return;

    if (mError.hasError())
    {
        out()->startObject("error");
        out()->write("message", mError.message());
        out()->write("details", mError.details());
        out()->write("code", mError.code());
    }
    out()->flush();

    if (server()->logLevel() != SrvLogLevel::None)
    {
        mFinishLogItem = log()->debug("Request '%1', result=%2, time=%3ms")
                .arg(path())
                .arg(mError.code())
                .arg(mTimer.elapsed());
        if (server()->logLevel() & SrvLogLevel::RequestData)
        {
            FormatterJson json;
            json.writeMap(this->requestContentType(), mRequestData);
            mFinishLogItem.arg("RequestData", QString(json.getResult()));
        }
        if (server()->logLevel() & SrvLogLevel::RequestRawData)
        {
            mFinishLogItem.arg("RequestRawData", requestRawData());
        }
        if (server()->logLevel() & SrvLogLevel::ResponseData)
        {
            mFinishLogItem.arg("ResponseData", QString(mResponseBuffer.buffer()));
        }
    }
    mFinished = true;
}

void Context::handlerStart(Handler* pHandler)
{
    mHandler = pHandler;
}

void Context::start(quint32 pRequestID)
{
    // main server thread

    mRequestID = pRequestID;
    updateLogHeader();
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
