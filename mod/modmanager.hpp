#pragma once
#include <QMap>
#include <QString>
#include "ctimemodule.hpp"

namespace Qk
{
namespace Mod
{

class ModManager
{
public:
    template<class TModule, bool pEnabled>
    TModule* createModule();
    void init();
    void run();

private:
    QMap<QString, CTimeModule*> mModules;
};

template<class TModule, bool pEnabled>
TModule* ModManager::createModule()
{
    if (pEnabled)
    {
        TModule* m = new TModule();
        mModules[m->name()] = m;
        return m;
    }
    return nullptr;
}

ModManager* mod();

}
}
