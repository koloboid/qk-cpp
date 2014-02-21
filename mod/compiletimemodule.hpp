#pragma once

#include <QString>

namespace Qk
{
namespace Mod
{

class ICompileTimeModule
{
public:
    ICompileTimeModule(const QString& pProjectName, const QString& pModName);
    virtual ~ICompileTimeModule();

public:
    QString name() const { return mModName; }
    QString projectName() const { return mProjectName; }
    QString fullName(const QString& pDivider = ".") const { return mProjectName + pDivider + mModName; }
    virtual void init();
    virtual void run();

private:
    QString mModName;
    QString mProjectName;
};

template<bool TEnabled>
class CompileTimeModule : public ICompileTimeModule
{
public:
    static constexpr const bool Enabled = TEnabled;

public:
    CompileTimeModule(const QString& pProjectName, const QString& pModName)
        : ICompileTimeModule(pProjectName, pModName)
    {
    }
};

}
}
