#pragma once

#include <QMap>
#include <QString>
#include <QVariant>
#include <QSharedPointer>
#include "dbmacro.hpp"

namespace Qk {
namespace Db {

class IField;
class IModel;
class Db;
typedef QSharedPointer<IModel> RModel;

class ITable
{
    friend class Db;

public:
    ITable(const QString& pName);
    virtual ~ITable();

public:
    void addField(IField* pField);
    virtual RModel newRow() const = 0;
    virtual QVariant newLinkRow(QVariant pId) const = 0;
    virtual RModel rowFromVariant(QVariant pVal) const = 0;
    virtual void init();

    QString name() const { return mName; }
    QString title() const { return mTitle; }
    QString description() const { return mDescription; }
    QString engine() const { return mEngine; }
    QMap<QString, IField*> fields() const { return mFields; }
    IField* field(const QString& pName) const { return mFields[pName]; }
    IField* primaryField() const { return mPrimaryField; }
    Db* db() const { return mDb; }

    ITable& engine(const QString& pDbEngineName) { mEngine = pDbEngineName; return *this; }
    IField* operator[](const QString& pName) const;

private:
    QString mName;
    QString mTitle;
    QString mEngine;
    QString mDescription;
    IField* mPrimaryField = nullptr;
    IField* mDisplayField = nullptr;
    IField* mJoinParentField = nullptr;
    QMap<QString, IField*> mFields;
    Db* mDb;
};

template<class TModel>
class Table : public ITable
{
public:
    Table(const QString& pName)
        : ITable(pName)
    {
    }

public:
    virtual void init() override
    {
        TModel::initTable();
        ITable::init();
    }
    virtual RModel newRow() const override { return QSharedPointer<TModel>(new TModel()); }
    virtual QVariant newLinkRow(QVariant pId) const override;
    virtual RModel rowFromVariant(QVariant pVal) const override;
};

template<class TModel>
QVariant Table<TModel>::newLinkRow(QVariant pId) const
{
    TModel* m = new TModel();
    this->primaryField()->set(m, pId);
    QSharedPointer<TModel> model(m);
    return QVariant::fromValue(model);
}

template<class TModel>
RModel Table<TModel>::rowFromVariant(QVariant pVal) const
{
    return pVal.value<QSharedPointer<TModel>>();
}

}
}
