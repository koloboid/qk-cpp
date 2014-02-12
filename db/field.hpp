#pragma once

#include <QString>
#include <QVariant>
#include "fieldflag.hpp"
//#include "row.hpp"
#include "condition.hpp"

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
//    QVariant get(const IRow& pRow) const { return pRow.get(*this); }
    bool readOnly() const { return mReadOnly; }
    virtual void init() = 0;
    inline Condition operator==(const QVariant& pValue) const { return Condition(this, Condition::OpEqual, pValue); }

protected:
    void initLinkedTo(const char* pTabName);

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
    static constexpr bool IsFK = false;

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
    Field& flag(EFieldFlag pFlag) { mFlag = (EFieldFlag)(mFlag | pFlag); return *this; }
    Field& flags(EFieldFlag pFlag) { mFlag = pFlag; return *this; }
    Field& min(const QVariant pMin) { mMinVariant = pMin; return *this; }
    Field& max(const QVariant pMax) { mMaxVariant = pMax; return *this; }
    Field& minmax(const QVariant& pMin, const QVariant& pMax) { mMaxVariant = pMax; mMinVariant = pMin; return *this; }
    Field& defaultValue(const TType& pValue) { mDefaultVariant = QVariant::fromValue<TType>(pValue); return *this; }
    TTable* table() const { return static_cast<TTable*>(mTable); }
    virtual void init();

public:
    inline Condition operator==(const TType& pValue) const { return Condition(this, Condition::OpEqual, QVariant::fromValue<TType>(pValue)); }
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

}

template<class TTable, class TType, bool TReadOnly>
void Field<TTable, TType, TReadOnly>::init()
{
    typedef typename std::conditional<std::is_convertible<TType, IRow>::value, ITable*, Traits::DummyName>::type TableType;
    const char* tabname = Traits::TableLink<TType, TableType>::get();
    initLinkedTo(tabname);
}

template<class TTable, class TType, bool TReadOnly>
QList<typename TTable::RowType> FieldFK<TTable, TType, TReadOnly>::fetch(const TType& pRow, Driver* pDrv) const
{
    return this->table()->select(pDrv).where(*this == pRow).list();
}

}
}
