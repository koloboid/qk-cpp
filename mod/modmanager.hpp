#pragma once
#include <QMap>
#include <QString>
#include "compiletimemodule.hpp"

namespace Qk
{
namespace Mod
{

class ModManager
{
public:
    template<class TModule>
    TModule* createModule();
    void init();
    void run();
    QMap<QString, ICompileTimeModule*> modules() const { return mModules; }

private:
    QMap<QString, ICompileTimeModule*> mModules;
};

template<class TModule>
TModule* ModManager::createModule()
{
    static_assert(std::is_base_of<Qk::Mod::ICompileTimeModule, TModule>::value, "TModule must be derived from Qk::Mod::CompileTimeModule");
    if (TModule::Enabled)
    {
        TModule* m = new TModule();
        mModules[m->fullName()] = m;
        return m;
    }
    return nullptr;
}

}
}
