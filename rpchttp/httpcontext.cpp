#include <QUuid>
#include <QUrl>
#include <QBuffer>
#include <QUrlQuery>
#include <QTextCodec>
#include <tufao-1/Tufao/Headers>
#include <qk/core/formatterjson.hpp>
#include <qk/core/formatterxml.hpp>
#include <qk/core/log.hpp>
#include <qk/rpc/server.hpp>
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
        QByteArray arr = mReq->headers().value("Cookie");
        QList<QNetworkCookie> list = QNetworkCookie::parseCookies(arr.replace(';', '\n'));
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
    if (server()->logLevel() & SrvLogLevel::RequestRawData)
    {
        FormatterJson json;
        foreach (QByteArray key, mReq->headers().keys())
        {
            json.write(QString(key), QString(mReq->headers().value(key)));
        }

        mFinishLogItem.arg("Headers", QString(json.getResult()));
    }
    emit onFinish();
}

void HttpContext::start(quint32 pRequestID)
{
    ASSERTPTR(mReq);

    mRequestRawData = mReq->readBody();
    Context::start(pRequestID);
}

void HttpContext::handlerStart(Handler *pHandler)
{
    Context::handlerStart(pHandler);
    parseRequest();
}

QString HttpContext::requestContentType() const
{
    if (mContentType.isEmpty())
    {
        mContentType = mReq->headers().value("Content-Type").toLower();
    }
    return mContentType;
}

void HttpContext::setCookie(const QString& pName, const QString& pVal, const QString& pPath, const QString& pDomain, bool pHttpOnly, bool pSecure)
{
    ASSERTSTR(pName, 1, 255);
    QString header = pName + "=" + pVal;
    if (!pPath.isEmpty()) header += "; Path=" + pPath;
    if (!pDomain.isEmpty()) header += "; Domain=" + pDomain;
    if (pHttpOnly) header += "; HttpOnly";
    if (pSecure) header += "; Secure";
    mResp->headers().insert("Set-Cookie", header.toUtf8());
}

void HttpContext::sendResponse()
{
    // main server thread
    if (mResp)
    {
        mResp->writeHead(statusCode(), statusText().toUtf8());
        if (session() && session()->isNew())
        {
            setCookie("QKSESSID", session()->id().toString(), "/");
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

void HttpContext::parseRequest()
{
    if (mReq->url().hasQuery())
    {
        QUrlQuery q(mReq->url());
        foreach (auto item, q.queryItems())
        {
            mRequestData[item.first] = item.second;
        }
    }
    QString type = requestContentType();
    if (type.contains("application/x-www-form-urlencoded"))
    {
        parsePostData();
    }
    else if (type.contains("application/json"))
    {
        parseJsonData();
    }
    else if (type.contains("application/xml"))
    {
        parseXmlData();
    }
}

void HttpContext::parsePostData()
{
    QString type = requestContentType();
    int csidx = type.indexOf("charset=");
    QTextCodec* codec = nullptr;
    if (csidx >= 0)
    {
        int csend = type.indexOf(";", csidx);
        csidx += 8;
        QString charset = type.mid(csidx, csend - csidx);
        codec = QTextCodec::codecForName(charset.toUtf8().data());
    }
    if (!codec) codec = QTextCodec::codecForName("utf-8");
    foreach (QByteArray pair, mRequestRawData.split('&'))
    {
        QList<QByteArray> expr = pair.split('=');
        QByteArray key = expr[0];
        QByteArray val = expr.size() > 1 ? expr[1] : QByteArray();
        QString skey = QUrl::fromPercentEncoding(key);
        QString sval = QUrl::fromPercentEncoding(val);
        mRequestData[skey] = sval;
    }
}

void HttpContext::parseJsonData()
{
    throw ErrorNotImplemented(ERRLOC);
}

void HttpContext::parseXmlData()
{
    throw ErrorNotImplemented(ERRLOC);
}

}
}
