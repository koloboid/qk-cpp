#include "qkconfig.hpp"

QkConfig::QkConfig(QObject* pParent)
    : QObject(pParent)
{
}

QkConfig::~QkConfig()
{
}

bool QkConfig::init(int pArgc, char** pArgv)
{
    return false;
}

bool QkConfig::handleHelp(QIODevice* pOutput)
{
    return false;
}

void QkConfig::sync()
{
}
