#pragma once

#include <QString>
#include <QVariant>
#include <QSharedPointer>
#include "table.hpp"
#include "fieldflag.hpp"
#include "condition.hpp"

namespace Qk {
namespace Db {

class IModel;
class IValue;

class IField
{
public:
    IField(const QString& pName, ITable* pTable, QMetaType::Type pType, size_t pOffset)
        : mName(pName), mTable(pTable), mType(pType), mOffset(pOffset)
    {
    }
    virtual ~IField()
    {
    }

public:
    QString title() const { return mTitle; }
    QString name() const { return mName; }
    QString description() const { return mDescription; }
    QMetaType::Type type() const { return mType; }
    FieldFlag flags() const { return mFlag; }
    QVariant min() const { return mMinVariant; }
    QVariant max() const { return mMaxVariant; }
    QVariant defaultValue() const { return mDefaultVariant; }
    ITable* table() const { return mTable; }
    ITable* linkedTo() const { return mLinkedTo; }

    QVariant get(const QSharedPointer<IModel>& pModel) const { return get(pModel.data()); }
    void set(const QSharedPointer<IModel>& pModel, QVariant pVal) const { set(pModel.data(), pVal); }
    QVariant get(const IModel* pModel) const;
    void set(const IModel* pModel, QVariant pVal) const;

//    virtual QString constraintText() const = 0;
    virtual void init();

protected:
    QString mName;
    QString mTitle;
    QString mDescription;
    FieldFlag mFlag = FieldFlag::Simple;
    QVariant mMinVariant;
    QVariant mMaxVariant;
    QVariant mDefaultVariant;
    bool mCheckRange = false;
    ITable* mTable = 0;
    ITable* mLinkedTo = 0;
    QMetaType::Type mType;
    size_t mOffset = 0;
};

template<class TFieldType, class TBase>
class TableLink
{
public:
    static constexpr ITable* get() { return nullptr; }
};
template<class TFieldType>
class TableLink<TFieldType, IModel*>
{
public:
    static constexpr ITable* get()
    {
        typedef typename TFieldType::element_type model;
        return &model::TableSchema;
    }
};

template<class TFieldType>
class Field : public IField
{
private:
    typedef typename std::conditional<std::is_convertible<TFieldType, QSharedPointer<IModel>>::value, IModel*, TFieldType>::type BaseType;


public:
    template<class TModel>
    Field(const QString& pName, Table<TModel>* pTable, IValue* pValuePtr)
        : IField(pName, pTable, (QMetaType::Type)qMetaTypeId<TFieldType>(), (quint8*)pValuePtr - (quint8*)(IModel*)((TModel*)0))
    {
        pTable->addField(this);
        mLinkedTo = TableLink<TFieldType, BaseType>::get();
    }

public:
    Field& title(const QString& pTitle) { mTitle = pTitle; return *this; }
    Field& description(const QString& pDescription) { mDescription = pDescription; return *this; }
    Field<TFieldType>& flag(const FieldFlag& pFlag) { mFlag = (FieldFlag)(mFlag | pFlag); return *this; }
    Field<TFieldType>& flags(const FieldFlag& pFlag) { mFlag = pFlag; return *this; }
    Field& min(const QVariant pMin) { mMinVariant = pMin; mCheckRange = true; return *this; }
    Field& max(const QVariant pMax) { mMaxVariant = pMax; mCheckRange = true; return *this; }
    Field& minmax(const QVariant& pMin, const QVariant& pMax) { mMaxVariant = pMax; mMinVariant = pMin; mCheckRange = true; return *this; }
    Field& defaultValue(const TFieldType& pValue) { mDefaultVariant = QVariant::fromValue<TFieldType>(pValue); return *this; }

public:
    inline Condition operator==(const TFieldType& pValue) const { return Condition(this, Condition::OpEqual, QVariant::fromValue<TFieldType>(pValue)); }
};

template<>
template<class TModel>
Field<QString>::Field(const QString& pName, Table<TModel>* pTable, IValue* pValuePtr)
    : IField(pName, pTable, (QMetaType::Type)qMetaTypeId<QString>(), (quint8*)pValuePtr - (quint8*)(IModel*)((TModel*)0))
{
    pTable->addField(this);
    this->minmax(0, 255);
}

}
}
