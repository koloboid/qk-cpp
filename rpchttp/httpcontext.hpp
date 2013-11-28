#pragma once

#include <QNetworkCookie>
#include <QBuffer>
#include <qk/rpc/context.hpp>
#include <tufao-1/Tufao/HttpServerRequest>
#include <tufao-1/Tufao/HttpServerResponse>

using namespace Tufao;

namespace Qk {
namespace Rpc {

class HttpContext : public Context
{
    Q_OBJECT

public:
    HttpContext(HttpServerRequest* pReq, HttpServerResponse* pResp);
    virtual ~HttpContext();

public:
    virtual QUuid sessionId() override;
    virtual QString path() override;
    virtual Formatter* out() override { ASSERTPTR(mOut); return mOut; }

    void ready();
    QNetworkCookie getCookie(const QString& pCookieName);

signals:
    void onFinish();

protected:
    virtual void finish() override;

protected slots:
    void sendResponse();
    void connectionTerminated();

private:
    HttpServerRequest* mReq;
    HttpServerResponse* mResp;
    QBuffer mResponseBuffer;
    Formatter* mOut = 0;
    QMap<QString, QNetworkCookie> mCookieMap;
};

}
}
