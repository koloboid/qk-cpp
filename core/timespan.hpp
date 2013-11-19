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

    TimeSpan(quint64 pTotalMsec)
        : mTotalMsec(pTotalMsec)
    {
    }

public:
    quint64 totalMsec() const { return mTotalMsec; }

private:
    quint64 mTotalMsec = -1;
};

}
}

Q_DECLARE_METATYPE(Qk::Core::TimeSpan)
