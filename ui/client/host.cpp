#include "host.hpp"
#include "messagebox.hpp"
#include <QTimer>
#include <QApplication>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <qk/core/log.hpp>

using namespace Qk::Core;

namespace Qk {
namespace Ui {
namespace Client {

Host::Host(const QUrl& pServerUrl, QWidget* pFailForm, QWidget* pSplashForm)
    : QObject(qApp)
{
    mServerUrl = pServerUrl;
    mFailForm = pFailForm;
    mSplashForm = pSplashForm;
    mNet = new QNetworkAccessManager(this);
}

void Host::start()
{
    QTimer::singleShot(0, this, SLOT(hostStarted()));
}

void Host::hostStarted()
{
    getUrl(mServerUrl);
}

void Host::gotError(QNetworkReply::NetworkError pErr)
{
    QNetworkReply* reply = (QNetworkReply*)sender();
    MessageBox::showError(reply);
}

void Host::gotResponse()
{
    QNetworkReply* reply = (QNetworkReply*)sender();
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) return;
    QByteArray data = reply->readAll();
    QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    if (contentType != "application/json")
    {
        log()->warn("Unsupported content-type recieved: %1").arg(contentType);
    }
    QJsonDocument doc = QJsonDocument::fromBinaryData(data);
    if (doc.isNull())
    {
        QMessageBox::critical(nullptr, tr("Ошибка"), tr("Ошибка JSON-документа"));
        return;
    }
    if (!doc.object().value("js").isString())
    {
    }
}

void Host::getUrl(const QUrl& pUrl)
{
    QNetworkReply* reply = mNet->get(QNetworkRequest(pUrl));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(gotError(QNetworkReply::NetworkError)));
    connect(reply, &QNetworkReply::finished, this, &Host::gotResponse);
}

}
}
}
