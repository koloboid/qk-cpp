#pragma once

#include <QMap>
#include <QString>
#include <QCoreApplication>

template<class TEnum>
class QkEnumDesc
{
public:
    QkEnumDesc(quint64 pValue, QString pName, QString pTitle, QString pDesc)
        : mValue(pValue), mName(pName), mTitle(pTitle), mDescription(pDesc)
    {
        TEnum::mDescMap[pValue] = this;
    }

    QkEnumDesc()
        : mValue(0)
    {
    }

    quint64 value() const { return mValue; }
    QString name() const { return mName; }
    QString title() const { return mTitle; }
    QString description() const { return mDescription; }

private:
    quint64 mValue;
    QString mName;
    QString mTitle;
    QString mDescription;
};

template<typename TEnum, class TBase>
class QkEnumItem
{
public:
    friend QkEnumDesc<TEnum>;
    class BaseType
    {
        friend TEnum;

    private:
        constexpr BaseType(TBase pValue)
            : mValue(pValue)
        {
        }

    public:
        constexpr operator TBase() const
        {
            return mValue;
        }

        explicit operator TEnum() const
        {
            return TEnum(mValue);
        }

        TEnum operator()() const
        {
            return TEnum(mValue);
        }

        TEnum getEnum() const
        {
            return TEnum(mValue);
        }

        BaseType operator|(const BaseType& pVal) const
        {
            return mValue | pVal.mValue;
        }

        BaseType operator~() const
        {
            return ~mValue;
        }

    private:
        TBase mValue;
    };

    QkEnumItem(BaseType pVal)
        : mValue(pVal)
    {
    }

    operator TBase() const
    {
        return mValue;
    }

    QString getName() const { return mDescMap[mValue]->name(); }
    QString getTitle() const { return mDescMap[mValue]->title(); }
    QString getDesc() const { return mDescMap[mValue]->description(); }
    bool hasOneOf(TEnum pVal) const { return mValue & pVal.mValue; }

protected:
    BaseType mValue;
    static QMap<TBase, QkEnumDesc<TEnum>*> mDescMap;

    class StaticInit
    {
    public:
        StaticInit();
    };
    static StaticInit mStaticInit;
};
