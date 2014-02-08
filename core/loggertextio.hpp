#pragma once

#include "core.export.hpp"
#include "logger.hpp"
#include "consoledevice.hpp"

namespace Qk
{
namespace Core
{

class QKCORE_EXPORT LoggerTextIO : public Logger
{
public:
    LoggerTextIO(QIODevice* pDevice, ELogLevel pLevelMask = LogLevel::MaskAll, bool pUseColor = false);

public:
    virtual void writeItem(const LogItem &pItem) override;

protected:
    void setConsoleColor(const LogItem &pItem);

    QIODevice* mDevice = nullptr;
    QFile* mFile = nullptr;
    ELogLevel mLevelMask;
    bool mUseColor = false;
};

}
}
