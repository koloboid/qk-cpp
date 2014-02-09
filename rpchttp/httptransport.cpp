#include <tufao-1/Tufao/HttpServerRequest>
#include <tufao-1/Tufao/HttpServerResponse>
#include <qk/core/error.hpp>
#include "httpcontext.hpp"
#include "httptransport.hpp"

using namespace Qk::Core;
using namespace Tufao;

namespace Qk {
namespace Rpc {

HttpTransport::HttpTransport(Server* pRpcServer, QHostAddress pListenAddr, quint16 pPOrt)
    : Transport(pRpcServer), mHttpServer(0), mListenAddr(pListenAddr), mListenPort(pPOrt)
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
        mHttpServer = new HttpServer(this);
        if (!mHttpServer->listen(mListenAddr, mListenPort))
        {
            throw Error(ERRLOC, tr("Невозможно установить HTTP сервер на %1:%2. Адрес занят другим сокетом")
                        .arg(mListenAddr.toString()).arg(mListenPort));
        }
        connect(mHttpServer, &HttpServer::requestReady, this, &HttpTransport::onRequestReady);
    }
}

void HttpTransport::stop()
{
    mIsPaused = false;
    if (mHttpServer) delete mHttpServer;
    mHttpServer = 0;
}

void HttpTransport::pause()
{
    mIsPaused = true;
}

void HttpTransport::onRequestReady(HttpServerRequest& pRequest, HttpServerResponse& pResp)
{
    if (!mIsPaused)
    {
        HttpContext* ctx = new HttpContext(server(), &pRequest, &pResp);
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
