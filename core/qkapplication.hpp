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
    QkApplication(int& pArgc, char** pArgv, const QString& pAppName, const QString& pCompanyName);

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
