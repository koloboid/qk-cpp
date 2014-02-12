#include "modmanager.hpp"

namespace Qk
{
namespace Mod
{

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
