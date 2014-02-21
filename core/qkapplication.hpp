#pragma once

#include <QCoreApplication>

namespace Qk
{
namespace Core
{

class QkApplication : public QCoreApplication
{
    Q_OBJECT

public:
    QkApplication(int& pArgc, char** pArgv, const QString& pCompanyName, const QString& pAppName);

public:
    virtual int exec();
    virtual bool notify(QObject* pDest, QEvent* pE) override;

signals:
    void appStart();

protected slots:
    virtual void runApp();
};

}
}
