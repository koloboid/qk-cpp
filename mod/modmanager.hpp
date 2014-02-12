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
    QMap<QString, CTimeModule*> modules() const { return mModules; }

private:
    QMap<QString, CTimeModule*> mModules;
};

template<class TModule, bool pEnabled>
TModule* ModManager::createModule()
{
    static_assert(std::is_base_of<Qk::Mod::CTimeModule, TModule>::value, "TModule must be derived from Qk::Mod::CTimeModule");
    if (pEnabled)
    {
        TModule* m = new TModule();
        mModules[m->fullName()] = m;
        return m;
    }
    return nullptr;
}

}
}
