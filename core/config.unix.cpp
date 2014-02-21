#include "config.hpp"
#include <QDir>

namespace Qk
{
namespace Core
{

bool Config::loadFromDefaultLocations(const QString& pFileName)
{
    Config::init();

    loadFile(QString("/etc/%1/%2/%3").arg(QCoreApplication::organizationName()).arg(QCoreApplication::applicationName())
             .arg(pFileName));
    loadFile(QString("%1/.config/%2/%3/%4").arg(QDir::homePath()).arg(QCoreApplication::organizationName())
             .arg(QCoreApplication::applicationName()).arg(pFileName));
    loadFile(QCoreApplication::applicationDirPath() + "./main.json");
    loadFile("./" + pFileName);

    if (Config::processCmdLine(QCoreApplication::arguments()))
    {
        return false;
    }

    if (!sampleConf().isEmpty())
    {
        saveJSON(sampleConf());
        mLog.info("Sample config created successfully");
        return false;
    }
    if (!config().isEmpty())
    {
        log()->info("Loading config from %1").arg(config());
        loadJSON(config());
    }

    return true;
}

}
}
