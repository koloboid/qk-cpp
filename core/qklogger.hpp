#pragma once

#include "qkcore.export.hpp"
#include "qklogitem.hpp"

class QKCORE_EXPORT QkLogger
{
public:
    QkLogger()
    {
    }

    virtual ~QkLogger()
    {
    }

public:
    virtual void writeItem(const QkLogItem& pItem) = 0;
};
