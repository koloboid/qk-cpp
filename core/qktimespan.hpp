#pragma once

#include "qkcore.export.hpp"

class QKCORE_EXPORT QkTimeSpan
{
public:
    QkTimeSpan()
    {
    }

    QkTimeSpan(quint64 pTotalMsec)
        : mTotalMsec(pTotalMsec)
    {
    }

public:
    quint64 totalMsec() const { return mTotalMsec; }

private:
    quint64 mTotalMsec = -1;
};
