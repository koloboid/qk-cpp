#include "ctimemodule.hpp"
#include <qk/core/log.hpp>

using namespace Qk::Core;

namespace Qk
{
namespace Mod
{

CTimeModule::CTimeModule(const QString& pProjectName, const QString& pModName)
    : mModName(pModName), mProjectName(pProjectName)
{

}

CTimeModule::~CTimeModule()
{

}

void CTimeModule::init()
{
    log()->info(TR("Модуль '%1' инициализирован").arg(name()));
}

void CTimeModule::run()
{
}

}
}
