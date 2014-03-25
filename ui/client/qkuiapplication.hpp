#pragma once

#include <QApplication>

namespace Qk {
namespace Ui {
namespace Client {

class QkUiApplication : public QApplication
{
    Q_OBJECT

public:
    QkUiApplication(int& pArgc, char** pArgv, const QString& pCompanyName, const QString& pAppName);

public:
    virtual int exec();
    virtual bool notify(QObject* pDest, QEvent* pE) override;

protected slots:
    virtual void runApp();
};

}
}
}
