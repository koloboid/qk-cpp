#pragma once

#include <QString>

namespace Qk
{
namespace Mod
{

class CTimeModule
{
public:
    CTimeModule(const QString& pProjectName, const QString& pModName);
    virtual ~CTimeModule();

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

}
}
