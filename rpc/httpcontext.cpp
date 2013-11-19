#include "httpcontext.hpp"
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <qk/core/log.hpp>

using namespace Qk::Core;

namespace Qk
{
namespace RPC
{

HttpContext::HttpContext(HttpServerRequest* pReq, HttpServerResponse* pResp, QObject* parent)
    : QObject(parent), mRequest(pReq), mResponse(pResp)
{
    connect(mRequest, SIGNAL(destroyed(QObject*)), SLOT(httpDestroyed()));
    connect(mResponse, SIGNAL(destroyed(QObject*)), SLOT(httpDestroyed()));
}

void HttpContext::httpDestroyed()
{
    mRequest = 0;
    mResponse = 0;
    // stop job
    // deleteLater();
}

void HttpContext::process()
{
    QtConcurrent::run(this, &HttpContext::job);
}

void HttpContext::job()
{
//    Thread::sleepMs(qrand() % 20);
    qApp->postEvent(this, new QEvent(QEvent::User));
}

void HttpContext::jobFinish()
{
    if (mResponse)
    {
//        mResponse->writeHead(Tufao::HttpResponseStatus::OK);
        //mResponse->headers().replace("Content-Type", "text/plain");
        //mResponse->end("Hello");
    }
    else
    {
        log()->warn("Response is null");
    }
    deleteLater();
}

bool HttpContext::event(QEvent* pE)
{
    if (pE && pE->type() == QEvent::User)
    {
        jobFinish();
    }
    return QObject::event(pE);
}

}
}
