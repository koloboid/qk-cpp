#include "modmanager.hpp"

namespace Qk
{
namespace Mod
{

void ModManager::init()
{
    foreach (ICompileTimeModule* mod, mModules.values())
    {
        mod->init();
    }
}

void ModManager::run()
{
    foreach (ICompileTimeModule* mod, mModules.values())
    {
        mod->run();
    }
}


}
}
