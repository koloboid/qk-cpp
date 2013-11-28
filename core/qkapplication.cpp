#include <QTranslator>
#include <QTimer>
#include "qkapplication.hpp"
#include "error.hpp"
#include "log.hpp"

namespace Qk
{
namespace Core
{

QkApplication::QkApplication(int& pArgc, char** pArgv, const QString& pAppName, const QString& pCompanyName)
    : QCoreApplication(pArgc, pArgv)
{
    setApplicationName(pAppName);
    setOrganizationName(pCompanyName);
    connect(this, &QkApplication::appStart, this, &QkApplication::runApp);
}

bool QkApplication::notify(QObject* pDest, QEvent* pE)
{
    try
    {
        return QCoreApplication::notify(pDest, pE);
    }
    catch (const Error& pErr)
    {
        log()->error(pErr);
        return false;
    }
    catch (const std::exception& pErr)
    {
        log()->error(pErr);
        return false;
    }
}

void QkApplication::runApp()
{
    log()->source("MAIN");
    log()->addStdLoggers();
    log()->start();
    log()->installQtHandler();
    log()->installSysSignalHandler();
    log()->info(applicationName() + " loaded.");
    log()->appendSystemInfo().direct();

    QTranslator* apptr = new QTranslator();
    if (apptr->load("qt_ru.qm", "/usr/share/qt/translations"))
    {
        installTranslator(apptr);
    }
    else
    {
        log()->warn(tr("Не удалось загрузить переводы Qt"));
    }
}

int QkApplication::exec()
{
    QTimer::singleShot(0, this, SIGNAL(appStart()));
    return QCoreApplication::exec();
}

}
}
