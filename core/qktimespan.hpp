#pragma once

#include "qkcore.export.hpp"
#include <QMetaType>

class QKCORE_EXPORT QkTimeSpan
{
public:
    static const QkTimeSpan minValue;
    static const QkTimeSpan maxValue;
    static QMetaType::Type metaTypeID();

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

Q_DECLARE_METATYPE(QkTimeSpan)
