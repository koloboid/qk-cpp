#pragma once

#include "qkthrowable.hpp"

class QkFutureBase : public QkThrowable
{
};

template<class TResult>
class QKCORE_EXPORT QkFuture : public QkFutureBase
{
public:
    using StdCallback = std::function<void(const QkError&, const TResult&)>;

public:
    QkFuture(const TResult& pResult);

public:
    TResult wait();
    QkFuture& done(const StdCallback& pCb);

    operator TResult() const;
};
