#pragma once

#include <QNetworkCookie>
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
    HttpContext(Server* pServer, HttpServerRequest* pReq, HttpServerResponse* pResp);
    virtual ~HttpContext();

public:
    virtual QUuid sessionId() override;
    virtual QString path() override;

    QNetworkCookie getCookie(const QString& pCookieName);

signals:
    void onFinish();

protected:
    virtual void finish() override;
    virtual void start(quint32 pRequestID) override;
    virtual void handlerStart(Handler* pHandler) override;
    virtual QByteArray requestRawData() const override { return mRequestRawData; }
    virtual QString requestContentType() const override;
    void setCookie(const QString& pName, const QString& pVal, const QString& pPath, const QString& pDomain = QString(),
                   bool pHttpOnly = true, bool pSecure = false);

protected slots:
    void sendResponse();
    void connectionTerminated();
    void parseRequest();
    void parsePostData();
    void parseJsonData();
    void parseXmlData();

private:
    HttpServerRequest* mReq;
    HttpServerResponse* mResp;
    QMap<QString, QNetworkCookie> mCookieMap;
    QByteArray mRequestRawData;
    mutable QString mContentType;
};

}
}
