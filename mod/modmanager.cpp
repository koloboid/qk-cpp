#include "modmanager.hpp"

namespace Qk
{
namespace Mod
{

ModManager* mod()
{
    static ModManager* mgr = new ModManager();
    return mgr;
}

void ModManager::init()
{
    foreach (CTimeModule* mod, mModules.values())
    {
        mod->init();
    }
}

void ModManager::run()
{
    foreach (CTimeModule* mod, mModules.values())
    {
        mod->run();
    }
}


}
}
