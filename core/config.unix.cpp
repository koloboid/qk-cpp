#include "config.hpp"
#include <QDir>

namespace Qk
{
namespace Core
{

bool Config::loadFromDefaultLocations()
{
    Config::init();

    loadFile(QString("/etc/%1/%2/main.json").arg(QCoreApplication::organizationName()).arg(QCoreApplication::applicationName()));
    loadFile(QString("%1/.config/%2/%3/main.json").arg(QDir::homePath()).arg(QCoreApplication::organizationName()).arg(QCoreApplication::applicationName()));
    loadFile(QCoreApplication::applicationDirPath() + "./main.json");
    loadFile("./main.json");

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
