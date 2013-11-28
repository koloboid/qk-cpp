#pragma once

#include <QHostAddress>
#include <tufao-1/Tufao/HttpServer>
#include <qk/rpc/transport.hpp>

namespace Qk {
namespace Rpc {

class HttpTransport : public Transport
{
public:
    HttpTransport(QHostAddress pListenAddr, quint16 pPOrt);
    virtual ~HttpTransport();

public:
    virtual void run();
    virtual void stop();
    virtual void pause();

private slots:
    void onRequestReady(Tufao::HttpServerRequest& pRequest, Tufao::HttpServerResponse& pResp);

private:
    Tufao::HttpServer* mServer;
    QHostAddress mListenAddr;
    quint16 mListenPort;
    bool mIsPaused = false;
};

}
}
