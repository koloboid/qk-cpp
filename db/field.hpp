#pragma once

#include <QString>
#include <QVariant>
#include <QDateTime>
#include <qk/core/error.hpp>
#include "fieldflag.hpp"
#include "condition.hpp"

using namespace Qk::Core;

namespace Qk {
namespace Db {

class ITable;
class Driver;
class IRow;

class IField
{
public:
    IField(ITable* pTable, const QString& pName, QMetaType::Type pType, EFieldFlag pFlag, bool pReadOnly);
    virtual ~IField();

public:
    QString title() const { return mTitle; }
    QString name() const { return mName; }
    QString description() const { return mDescription; }
    QMetaType::Type type() const { return mType; }
    EFieldFlag flags() const { return mFlag; }
    QVariant min() const { return mMinVariant; }
    QVariant max() const { return mMaxVariant; }
    QVariant defaultValue() const { return mDefaultVariant; }
    ITable* linkedTo() const { return mLinkedTo; }
    ITable* table() const { return mTable; }
    QString tableName() const;
    bool readOnly() const { return mReadOnly; }
    inline Condition operator==(const QVariant& pValue) const { return Condition(this, Condition::OpEqual, pValue); }

    virtual void init() = 0;
    virtual Error validateValue(const QVariant& pValue, bool pThrow = true) const = 0;
    virtual bool isEnum() const = 0;
    virtual int sizeOf() const = 0;
    QMetaType::Type underylingType() const;

protected:
    void initLinkedTo(const char* pTabName);
    static Error throwNow(bool pThrow, const QString& pMessage);

protected:
    QString mName;
    QString mTitle;
    QString mDescription;
    QVariant mMinVariant;
    QVariant mMaxVariant;
    QVariant mDefaultVariant;
    EFieldFlag mFlag = FieldFlag::Simple;
    ITable* mTable;
    QMetaType::Type mType;
    ITable* mLinkedTo = 0;
    bool mReadOnly;
};

template<class TTable, class TType, bool TReadOnly = false>
class Field : public IField
{
public:
    static constexpr const bool IsFK = false;

public:
    typedef TType Type;
    typedef TTable Table;
    static constexpr bool ReadOnly = TReadOnly;

public:
    Field(TTable* pTable, const QString& pName, EFieldFlag pFlags = FieldFlag::Simple)
        : IField(pTable, pName, (QMetaType::Type)qMetaTypeId<TType>(), pFlags, TReadOnly)
    {
        mFlag = pFlags;
        pTable->addField(this);
    }

public:
    Field& title(const QString& pTitle) { mTitle = pTitle; return *this; }
    Field& description(const QString& pDescription) { mDescription = pDescription; return *this; }
    Field& addFlag(EFieldFlag pFlag) { mFlag = (EFieldFlag)(mFlag | pFlag); return *this; }
    Field& flags(EFieldFlag pFlag) { mFlag = pFlag; return *this; }
    Field& min(const QVariant pMin) { mMinVariant = pMin; return *this; }
    Field& max(const QVariant pMax) { mMaxVariant = pMax; return *this; }
    Field& minmax(const QVariant& pMin, const QVariant& pMax) { mMaxVariant = pMax; mMinVariant = pMin; return *this; }
    Field& defaultValue(const TType& pValue) { mDefaultVariant = QVariant::fromValue<TType>(pValue); return *this; }
    TTable* table() const { return static_cast<TTable*>(mTable); }

    virtual void init();
    virtual Error validateValue(const QVariant& pValue, bool pThrow = true) const override;
    virtual bool isEnum() const override { return std::is_enum<TType>::value; }
    virtual int sizeOf() const override { return sizeof(TType); }

public:
    inline Condition operator==(const TType& pValue) const { return Condition(this, Condition::OpEqual, QVariant::fromValue<TType>(pValue)); }
    inline Condition operator^=(const TType& pValue) const { static_assert(std::is_same<TType, QString>::value, "Case-sensitivity comparison on non-string field is prohibited");
        return Condition(this, Condition::OpEqualCaseInsensitive, QVariant::fromValue<TType>(pValue)); }
};

template<class TTable, class TType>
using FieldRO = Field<TTable, TType, true>;

template<class TTable, class TType, bool TReadOnly = false>
class FieldFK : public Field<TTable, TType, TReadOnly>
{
public:
    static constexpr bool IsFK = true;

public:
    FieldFK(TTable* pTable, const QString& pName, EFieldFlag pFlags = FieldFlag::RefStrong)
        : Field<TTable, TType, TReadOnly>(pTable, pName, pFlags)
    {
    }

public:
    inline QList<typename TTable::RowType> fetch(const TType& pRow, Driver* pDrv = 0) const;

public:
    inline Condition operator==(const TType& pValue) const { return Condition(this, Condition::OpEqual, QVariant::fromValue<TType>(pValue)); }
};

namespace Traits
{

class DummyName
{
};

template<class TFieldType, class TBase>
class TableLink
{
public:
    static constexpr const char* get() { return nullptr; }
};
template<class TFieldType>
class TableLink<TFieldType, ITable*>
{
public:
    static constexpr const char* get() { return TFieldType::TableType::tableName; }
};

template<class TType>
inline Error validateWithOperators(const QVariant &pValue, const IField* pFld)
{
    TType v = pValue.value<TType>();
    if (pFld->max().isValid() && pFld->min().isValid())
    {
        if (v < pFld->min().value<TType>() || v > pFld->max().value<TType>())
            return ErrorBadArgument(ERRLOC, TR("Значение поля '%1' таблицы '%2' должно быть между '%3' и '%4'")
                             .arg(pFld->name()).arg(pFld->tableName()).arg(pFld->min().toString()).arg(pFld->max().toString()));
    }
    else
    {
        if (pFld->max().isValid())
        {
            if (v > pFld->max().value<TType>()) return ErrorBadArgument(ERRLOC, TR("Значение поля '%1' таблицы '%2' должно быть меньше чем '%3'")
                                                 .arg(pFld->name()).arg(pFld->tableName()).arg(pFld->max().toString()));
        }
        if (pFld->min().isValid())
        {
            if (v < pFld->min().value<TType>()) return ErrorBadArgument(ERRLOC, TR("Значение поля '%1' таблицы '%2' должно быть больше чем '%3'")
                                                 .arg(pFld->name()).arg(pFld->tableName()).arg(pFld->min().toString()));
        }
    }
    return Error();
}

template<class TType>
class Validator
{
public:
    static inline Error test(const QVariant &pValue, const IField* pFld) { return validateWithOperators<TType>(pValue, pFld); }
};

template<>
class Validator<QString>
{
public:
    static inline Error test(const QVariant &pValue, const IField* pFld) { return validateWithOperators<int>(pValue.toString().size(), pFld); }
};

}

template<class TTable, class TType, bool TReadOnly>
void Field<TTable, TType, TReadOnly>::init()
{
    typedef typename std::conditional<std::is_convertible<TType, IRow>::value, ITable*, Traits::DummyName>::type TableType;
    const char* tabname = Traits::TableLink<TType, TableType>::get();
    initLinkedTo(tabname);
}

template<class TTable, class TType, bool TReadOnly>
Error Field<TTable, TType, TReadOnly>::validateValue(const QVariant &pValue, bool pThrow) const
{
    Error err = Traits::Validator<TType>::test(pValue, this);
    if (pThrow && err.hasError()) throw err;
    return Error();
}

template<class TTable, class TType, bool TReadOnly>
QList<typename TTable::RowType> FieldFK<TTable, TType, TReadOnly>::fetch(const TType& pRow, Driver* pDrv) const
{
    return this->table()->select(pDrv).where(*this == pRow).list();
}

}
}
