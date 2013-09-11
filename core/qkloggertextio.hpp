#pragma once

#include "qkcore.export.hpp"
#include "qklogger.hpp"
#include "qkconsoledevice.hpp"

class QKCORE_EXPORT QkLoggerTextIO : public QkLogger
{
public:
    QkLoggerTextIO(QIODevice* pDevice, QkLogLevel pLevelMask = QkLogLevel::MaskAll, bool pUseColor = false);

public:
    virtual void writeItem(const QkLogItem &pItem) override;

protected:
    void setConsoleColor(const QkLogItem &pItem);

    QIODevice* mDevice = nullptr;
    QFile* mFile = nullptr;
    QkLogLevel mLevelMask;
    bool mUseColor = false;
};
