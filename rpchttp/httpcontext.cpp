#include <QUuid>
#include <QUrl>
#include <QBuffer>
#include <tufao-1/Tufao/Headers>
#include <qk/core/formatterjson.hpp>
#include <qk/core/formatterxml.hpp>
#include <qk/core/log.hpp>
#include "httpcontext.hpp"

using namespace Qk::Core;

namespace Qk {
namespace Rpc {

HttpContext::HttpContext(Server* pServer, HttpServerRequest* pReq, HttpServerResponse* pResp)
    : Context(pServer), mReq(pReq), mResp(pResp)
{
    connect(this, &HttpContext::onFinish, this, &HttpContext::sendResponse);
    connect(pReq, &HttpServerRequest::destroyed, this, &HttpContext::connectionTerminated);
    connect(pResp, &HttpServerResponse::destroyed, this, &HttpContext::connectionTerminated);
}

HttpContext::~HttpContext()
{
}

QUuid HttpContext::sessionId()
{
    QNetworkCookie cookie = getCookie("QKSESSID");
    if (cookie.name().size() > 0)
    {
        return QUuid(QString(cookie.value()));
    }
    else return QUuid();
}

QString HttpContext::path()
{
    ASSERTPTR(mReq);

    return mReq->url().path();
}

QNetworkCookie HttpContext::getCookie(const QString& pCookieName)
{
    ASSERTPTR(mReq);

    if (mCookieMap.isEmpty())
    {
        QList<QNetworkCookie> list = QNetworkCookie::parseCookies(mReq->headers().value("Cookie"));
        foreach (QNetworkCookie cookie, list)
        {
            mCookieMap[cookie.name()] = cookie;
        }
    }
    return mCookieMap.value(pCookieName);
}

void HttpContext::finish()
{
    // handler thread
    Context::finish();
    emit onFinish();
}

void HttpContext::start()
{
    ASSERTPTR(mReq);

    Context::start();
}

void HttpContext::sendResponse()
{
    // main server thread
    if (mResp)
    {
        mResp->writeHead(statusCode(), statusText().toUtf8());
        if (session() && session()->isNew())
        {
            mResp->headers().insert("Set-Cookie", ("QKSESSID=" + session()->id().toString()).toUtf8());
        }
        mResp->headers().insert("Content-Type", out()->contentType().toUtf8());
        mResp->end(mResponseBuffer.buffer());
    }
    else
    {
        log()->warn(tr("Подключение было сброшено, HTTP-клиент отказался от ответа"));
    }
    if (session()) session()->requestProcessed(this);
    deleteLater();
}

void HttpContext::connectionTerminated()
{
    // main server thread
    mResp = nullptr;
    mReq = nullptr;
}

}
}
