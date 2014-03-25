#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>

namespace Qk {
namespace Ui {
namespace Client {

class Host : public QObject
{
    Q_OBJECT

public:
    Host(const QUrl& pServerUrl, QWidget* pFailForm, QWidget* pSplashForm);

public:
    void start();

private slots:
    void hostStarted();
    void gotError(QNetworkReply::NetworkError pErr);
    void gotResponse();

private:
    void getUrl(const QUrl& pUrl);

private:
    QUrl mServerUrl;
    QWidget* mFailForm = nullptr;
    QWidget* mSplashForm = nullptr;
    QNetworkAccessManager* mNet = nullptr;
};

}
}
}
