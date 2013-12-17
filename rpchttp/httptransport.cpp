#include <tufao-1/Tufao/HttpServerRequest>
#include <tufao-1/Tufao/HttpServerResponse>
#include <qk/core/error.hpp>
#include "httpcontext.hpp"
#include "httptransport.hpp"

using namespace Qk::Core;
using namespace Tufao;

namespace Qk {
namespace Rpc {

HttpTransport::HttpTransport(QHostAddress pListenAddr, quint16 pPOrt)
    : mServer(0), mListenAddr(pListenAddr), mListenPort(pPOrt)
{
}

HttpTransport::~HttpTransport()
{
    stop();
}

void HttpTransport::run()
{
    if (mIsPaused)
    {
        mIsPaused = false;
    }
    else
    {
        stop();
        mIsPaused = false;
        mServer = new HttpServer(this);
        if (!mServer->listen(mListenAddr, mListenPort))
        {
            throw Error(ERRLOC, tr("Невозможно установить HTTP сервер на %1:%2. Адрес занят другим сокетом")
                        .arg(mListenAddr.toString()).arg(mListenPort));
        }
        connect(mServer, &HttpServer::requestReady, this, &HttpTransport::onRequestReady);
    }
}

void HttpTransport::stop()
{
    mIsPaused = false;
    if (mServer) delete mServer;
    mServer = 0;
}

void HttpTransport::pause()
{
    mIsPaused = true;
}

void HttpTransport::onRequestReady(HttpServerRequest& pRequest, HttpServerResponse& pResp)
{
    if (!mIsPaused)
    {
        HttpContext* ctx = new HttpContext(&pRequest, &pResp);
        connect(&pRequest, &HttpServerRequest::end, [this, ctx] {
            emit onRequest(this, ctx);
        });
    }
    else
    {
        pResp.writeHead(505, tr("Сервер приостановлен").toUtf8());
        pResp.end();
    }
}

}
}
