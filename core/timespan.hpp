#pragma once

#include "core.export.hpp"
#include <QMetaType>

namespace Qk
{
namespace Core
{

class QKCORE_EXPORT TimeSpan
{
public:
    static const TimeSpan minValue;
    static const TimeSpan maxValue;
    static QMetaType::Type metaTypeID();

public:
    TimeSpan()
    {
    }

    TimeSpan(qint64 pTotalMsec)
        : mTotalMsec(pTotalMsec)
    {
    }

public:
    qint64 totalMsec() const { return mTotalMsec; }

private:
    qint64 mTotalMsec = -1;
};

}
}

Q_DECLARE_METATYPE(Qk::Core::TimeSpan)
