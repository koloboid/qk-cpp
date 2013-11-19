#pragma once

#include <QObject>
#include <QEvent>
#include <tufao-1/Tufao/HttpServer>
#include <tufao-1/Tufao/HttpServerRequest>
#include <tufao-1/Tufao/HttpServerResponse>
#include <tufao-1/Tufao/Headers>
#include <QUrl>

using namespace Tufao;

namespace Qk
{
namespace RPC
{

class HttpContext : public QObject
{
    Q_OBJECT

public:
    explicit HttpContext(HttpServerRequest* pReq, HttpServerResponse* pResp, QObject* parent = 0);
    void process();
    virtual bool event(QEvent* pE) override;

protected slots:
    void httpDestroyed();
    void job();
    void jobFinish();

private:
    HttpServerRequest* mRequest;
    HttpServerResponse* mResponse;
};

}
}
