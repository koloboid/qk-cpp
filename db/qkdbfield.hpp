#pragma once

#include <QtCore/QSharedData>
#include <qkerror.hpp>
#include <qkblob.hpp>
#include "qkdbrow.hpp"
#include "qkdbfieldflag.hpp"

class QkDbTable;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbField
{
    Q_DISABLE_COPY(QkDbField)

public:
    QkDbField(QkDbTable* pTable, QMetaType::Type pType, QString pName, QString pTitle, QString pDescription,
              QVariant pDefaultValue, QkDbFieldFlag pFlag);
    virtual ~QkDbField();

    bool IsCounter() const;
    bool IsAutoIncrement() const { return HasFlag(QkDbFieldFlag::AutoIncrement); }
    bool IsFilterable() const;
    bool IsIterableJoin() const;
    bool IsVisible() const;
    bool IsVisibleJoin() const;
    bool IsPrimary() const { return HasFlag(EFieldFlag::Primary); }
    bool IsRefStrong() const { return HasFlag(EFieldFlag::RefStrong); }
    bool IsRefWeak() const;
    bool IsRef() const;
    bool IsPassword() const	{ return HasFlag(EFieldFlag::Password); }
    bool IsCascadeDelete() const { return HasFlag(EFieldFlag::CascadeDelete); }
    bool IsHistoryDisabled() const { return HasFlag(EFieldFlag::HistoryDisabled); }
    bool HasFlag(EFieldFlag pFlag) const { return Flag & pFlag; }

    virtual void Link();
    virtual void CheckValue(DbValue pValue) const;

    DbValue Get(const DbRow& pRow) const
    {
        return pRow.Get(*this);
    }
    void Set(DbRow& pRow, DbValue pValue) const
    {
        pRow.Set(*this, pValue);
    }

public:
    DbCondition operator==(DbValue pValue) const
    {
        return DbCondition(*this, DbOperator::Equal, pValue);
    }
    DbCondition operator%(QString pString) const
    {
        return DbCondition(*this, DbOperator::Like, pString);
    }
    DbValue operator()(const DbRow& pRow) const
    {
        return Get(pRow);
    }
    void operator()(DbRow& pRow, DbValue pValue) const
    {
        Set(pRow, pValue);
    }

private:
    const DbTable& Table;
    const EDbType Type;
    const QString Name;
    const QString Title;
    const QString Description;
    const DbValue DefaultValue;
    const EFieldFlag Flag;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TType>
class QkDbFieldGeneric : public QkDbField
{
public:
    QkDbFieldGeneric(DbTable& pTable, EDbType pType, QString pName, QString pTitle = QString(), QString pDescription = QString(),
                   TType pDefaultValue = DbValue(), EFieldFlag pFlag = EFieldFlag::Simple)
        : QkDbField(pTable, pType, pName, pTitle, pDescription, pDefaultValue, pFlag)
    {
    }

public:
    TType Get(const DbRow& pRow) const
    {
        return (TType)QkDbField::Get(pRow);
    }
    void Set(DbRow& pRow, TType pValue) const
    {
        QkDbField::Set(pRow, pValue);
    }

    TType operator()(const DbRow& pRow) const
    {
        return Get(pRow);
    }
    void operator()(DbRow& pRow, TType pValue) const
    {
        Set(pRow, pValue);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldString : public QkDbField
{
public:
    QkDbFieldString(DbTable& pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(), quint32 pMinSize = 0,
                  quint32 pMaxSize = 256, QString pDefault = QString(), EFieldFlag pFlag = (EFieldFlag)(EFieldFlag::Simple | EFieldFlag::Filterable))
        : QkDbField(pTable, EDbType::String, pName, pTitle, pDescription, pDefault, pFlag),
          mMinSize(pMinSize), mMaxSize(pMaxSize)
    {
    }

public:
    virtual void CheckValue(DbValue pValue) const override;
    quint32 MinSize() const
    {
        return mMinSize;
    }
    quint32 MaxSize() const
    {
        return mMaxSize;
    }

    QString Get(const DbRow& pRow) const
    {
        return QkDbField::Get(pRow).AsString();
    }
    void Set(DbRow& pRow, QString pValue) const
    {
        QkDbField::Set(pRow, pValue);
    }

    QString operator()(const DbRow& pRow) const
    {
        return Get(pRow);
    }
    void operator()(DbRow& pRow, QString pValue) const
    {
        Set(pRow, pValue);
    }

private:
    quint32 mMinSize;
    quint32 mMaxSize;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldURL : public QkDbFieldString
{
public:
    QkDbFieldURL(DbTable& pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(),
               QString pDefault = QString(), EFieldFlag pFlag = EFieldFlag::Simple)
        : QkDbFieldString(pTable, pName, pTitle, pDescription, 0, 256, pDefault, pFlag)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldImage : public QkDbFieldString
{
public:
    QkDbFieldImage(DbTable& pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(),
                 QString pDefault = QString(), EFieldFlag pFlag = EFieldFlag::Simple)
        : QkDbFieldString(pTable, pName, pTitle, pDescription, 0, 256, pDefault, pFlag)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldDescription : public QkDbFieldString
{
public:
    QkDbFieldDescription(DbTable& pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(),
                       QString pDefault = QString(), EFieldFlag pFlag = (EFieldFlag)(EFieldFlag::Simple | EFieldFlag::Filterable))
        : QkDbFieldString(pTable, pName, pTitle, pDescription, 0, 4096, pDefault, pFlag)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldBool : public QkDbFieldGeneric<bool>
{
public:
    QkDbFieldBool(DbTable& pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(),
                bool pDefault = false, EFieldFlag pFlag = EFieldFlag::Simple)
        : QkDbFieldGeneric<bool>(pTable, EDbType::Bool, pName, pTitle, pDescription, pDefault, pFlag)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldTimeSpan : public QkDbFieldGeneric<TimeSpan>
{
public:
    QkDbFieldTimeSpan(DbTable& pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(), TimeSpan pMin = TimeSpan::MinValue,
                    TimeSpan pMax = TimeSpan::MaxValue, TimeSpan pDefault = TimeSpan::MinValue, EFieldFlag pFlag = EFieldFlag::Simple)
        : QkDbFieldGeneric<TimeSpan>(pTable, EDbType::TimeSpan, pName, pTitle, pDescription, pDefault, pFlag),
          mMinValue(pMin), mMaxValue(pMax)
    {
    }

public:
    virtual void CheckValue(DbValue pValue) const override;
    TimeSpan MinValue() const
    {
        return mMinValue;
    }
    TimeSpan MaxValue() const
    {
        return mMaxValue;
    }

private:
    TimeSpan mMinValue;
    TimeSpan mMaxValue;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TRefTable>
class QkDbFieldLink : public QkDbFieldGeneric<DbRow>
{
public:
    QkDbFieldLink(DbTable& pTable, Db& pDb, QString pName, QString pTitle = QString(), QString pDescription = QString(), EFieldFlag pFlag = EFieldFlag::RefStrong)
        : QkDbFieldGeneric<DbRow>(pTable, DbType::Link, pName, pTitle, pDescription, DbRow(pTable), pFlag)
    {
    }

public:
    const TRefTable* Join;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldPK : public QkDbFieldGeneric<DbRowID>
{
public:
    QkDbFieldPK(DbTable& pTable, QString pName = "ID", QString pTitle = TR("Идентификатор"), QString pDescription = QString())
        : QkDbFieldGeneric<DbRowID>(pTable, DbType::UInt64, pName, pTitle, pDescription, 0, (EFieldFlag)(EFieldFlag::Primary | EFieldFlag::AutoIncrement))
    {
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TEnum, EDbType TBaseType>
class QkDbFieldEnum : public QkDbField
{
public:
    QkDbFieldEnum(DbTable& pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(), EFieldFlag pFlag = EFieldFlag::Simple)
        : QkDbField(pTable, (EDbType)TBaseType, pName, pTitle, pDescription, 0, pFlag)
    {
    }

public:
    TEnum Get(const DbRow& pRow) const
    {
        return TEnum(QkDbField::Get(pRow).AsUInt64());
    }
    void Set(DbRow& pRow, const TEnum& pValue) const
    {
        QkDbField::Set(pRow, pValue.AsInt());
    }
    TEnum operator()(const DbRow& pRow) const
    {
        return Get(pRow);
    }
    void operator()(DbRow& pRow, const TEnum& pValue) const
    {
        QkDbField::Set(pRow, pValue.AsInt());
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldBlob : public QkDbFieldGeneric<Blob>
{
public:
    QkDbFieldBlob(DbTable& pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(), quint32 pMinSize = 0,
                quint32 pMaxSize = 1024, Blob pDefault = Blob(), EFieldFlag pFlag = EFieldFlag::Simple)
        : QkDbFieldGeneric<Blob>(pTable, EDbType::UInt8, pName, pTitle, pDescription, pDefault, pFlag),
          mMinSize(pMinSize), mMaxSize(pMaxSize)
    {
    }

public:
    virtual void CheckValue(DbValue pValue) const override;
    quint32 MinSize() const
    {
        return mMinSize;
    }
    quint32 MaxSize() const
    {
        return mMaxSize;
    }

    Blob operator()(const DbRow& pRow) const
    {
        return Get(pRow);
    }

private:
    quint32 mMinSize;
    quint32 mMaxSize;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldDateTime : public QkDbFieldGeneric<QDateTime>
{
public:
    QkDbFieldDateTime(DbTable& pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(),
                    QDateTime pMin = QDateTime::fromMSecsSinceEpoch(std::numeric_limits<qint64>::min()),
                    QDateTime pMax = QDateTime::fromMSecsSinceEpoch(std::numeric_limits<qint64>::max()),
                    QDateTime pDefault = QDateTime::currentDateTime(), EFieldFlag pFlag = EFieldFlag::Simple)
        : QkDbFieldGeneric<QDateTime>(pTable, EDbType::DateTime, pName, pTitle, pDescription, pDefault, pFlag),
          mMinValue(pMin), mMaxValue(pMax)
    {
    }

public:
    virtual void CheckValue(DbValue pValue) const override;
    QDateTime MinValue() const
    {
        return mMinValue;
    }
    QDateTime MaxValue() const
    {
        return mMaxValue;
    }

private:
    QDateTime mMinValue;
    QDateTime mMaxValue;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TNumber, EDbType TType>
class QkDbFieldNumber : public QkDbFieldGeneric<TNumber>
{
public:
    QkDbFieldNumber(DbTable& pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(),
                  TNumber pMin = std::numeric_limits<TNumber>::min(),
                  TNumber pMax = std::numeric_limits<TNumber>::max(),
                  TNumber pDefault = 0, EFieldFlag pFlag = EFieldFlag::Simple)
        : QkDbFieldGeneric<TNumber>(pTable, (EDbType)TType, pName, pTitle, pDescription, pDefault, pFlag),
          mMinValue(pMin), mMaxValue(pMax)
    {
    }

public:
    virtual void CheckValue(DbValue pValue) const override
    {
        TNumber num = pValue;

        if (num < mMinValue) throw ErrorFieldCheck(ERRLOC, TR("Поле '%1' должно быть больше чем '%2'").arg(this->Title).arg(mMinValue));

        if (num > mMaxValue) throw ErrorFieldCheck(ERRLOC, TR("Поле '%1' должно быть не более чем '%2'").arg(this->Title).arg(mMaxValue));

            QkDbField::CheckValue(pValue);
        }

        TNumber MinSize() const
            {
                return mMinValue;
            }
    TNumber MaxSize() const
    {
        return mMaxValue;
    }

private:
    TNumber mMinValue;
    TNumber mMaxValue;
};

typedef QkDbFieldNumber<quint8, DbType::UInt8> QkDbFieldUInt8;
typedef QkDbFieldNumber<quint16, DbType::UInt16> QkDbFieldUInt16;
typedef QkDbFieldNumber<quint32, DbType::UInt32> QkDbFieldUInt32;
typedef QkDbFieldNumber<quint64, DbType::UInt64> QkDbFieldUInt64;

typedef QkDbFieldNumber<qint8, DbType::Int8> QkDbFieldInt8;
typedef QkDbFieldNumber<qint16, DbType::Int16> QkDbFieldInt16;
typedef QkDbFieldNumber<qint32, DbType::Int32> QkDbFieldInt32;
typedef QkDbFieldNumber<qint64, DbType::Int64> QkDbFieldInt64;

typedef QkDbFieldNumber<float, DbType::Float> QkDbFieldFloat;
typedef QkDbFieldNumber<double, DbType::Double> QkDbFieldDouble;
