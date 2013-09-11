#pragma once

#include "qkcore.export.hpp"
#include "qkerror.hpp"

class QKCORE_EXPORT QkThrowable
{
public:
#ifdef QK_EXCEPTION
    QkThrowable(bool pThrowException = true)
#else
    QkThrowable(bool pThrowException = false)
#endif
        : mError(), mThrowException(pThrowException)
    {
    }

    virtual ~QkThrowable()
    {
    }

public:
    const QkError& lastError() const { return mError; }

protected:
    bool throwNow(const QkError& pError)
    {
        mError = pError;
#ifdef QK_EXCEPTION
        if (pError.hasError() && mThrowException) throw mError;
#endif
        return false;
    }

    void resetError()
    {
        mError = QkError();
    }

private:
    QkError mError;
    bool mThrowException;
};
