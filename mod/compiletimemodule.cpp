#include "compiletimemodule.hpp"
#include <qk/core/log.hpp>

using namespace Qk::Core;

namespace Qk
{
namespace Mod
{

ICompileTimeModule::ICompileTimeModule(const QString& pProjectName, const QString& pModName)
    : mModName(pModName), mProjectName(pProjectName)
{

}

ICompileTimeModule::~ICompileTimeModule()
{

}

void ICompileTimeModule::init()
{
    log()->info(TR("Модуль '%1' инициализирован").arg(fullName()));
}

void ICompileTimeModule::run()
{
}

}
}
