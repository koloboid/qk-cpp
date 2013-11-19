#pragma once

#include <QVariant>

namespace Qk {
namespace Db {

template<class T>
class Field;
class IModel;

class IValue
{
    friend class IField;

public:
    bool isChanged() const { return mChanged; }

protected:
    bool mChanged = false;
    QVariant mValue;
};

template<class T>
class Value : public IValue
{
private:
    Value(const T& pValue)
    {
    }

public:
    Value(Field<T>*, const T& pDefault = T())
    {
        mValue = QVariant::fromValue<T>(pDefault);
    }

    const T operator()() const
    {
        return mValue.value<T>();
    }

    void operator()(const T& pValue)
    {
        mValue = QVariant::fromValue(pValue);
        mChanged = true;
    }
};

template<class T, class TFriend>
class ValueRO : public IValue
{
    friend TFriend;

private:
    ValueRO(const T& pValue)
    {
    }

public:
    ValueRO(Field<T>*, const T& pDefault = T())
    {
        mValue = QVariant::fromValue<T>(pDefault);
    }

    const T operator()() const
    {
        return mValue.value<T>();
    }

protected:
    void operator()(const T& pValue)
    {
        mValue = QVariant::fromValue(pValue);
        mChanged = true;
    }
};

}
}
