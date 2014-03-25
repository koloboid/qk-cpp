#include <QTranslator>
#include <QTimer>
#include "qkuiapplication.hpp"
#include <qk/core/error.hpp>
#include <qk/core/log.hpp>

using namespace Qk::Core;

inline void initMyResource() { Q_INIT_RESOURCE(res); }

namespace Qk {
namespace Ui {
namespace Client {

QkUiApplication::QkUiApplication(int& pArgc, char** pArgv, const QString& pCompanyName, const QString& pAppName)
    : QApplication(pArgc, pArgv)
{
    setApplicationName(pAppName);
    setOrganizationName(pCompanyName);
    initMyResource();
}

bool QkUiApplication::notify(QObject* pDest, QEvent* pE)
{
    try
    {
        return QApplication::notify(pDest, pE);
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

void QkUiApplication::runApp()
{
    log()->source("MAIN");
    log()->addStdLoggers();
    log()->start();
    log()->installQtHandler();
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

int QkUiApplication::exec()
{
    QTimer::singleShot(0, this, SLOT(runApp()));
    return QApplication::exec();
}

}
}
}
