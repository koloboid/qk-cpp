#pragma once
#include <QString>

namespace Qk
{
namespace Mod
{

class CTimeModule
{
public:
    CTimeModule();
    virtual ~CTimeModule();

public:
    virtual QString name() const = 0;
    virtual void init();
    virtual void run();
};

}
}
