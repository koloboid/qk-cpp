#pragma once

#include "qkdb.export.hpp"
#include "qkdbtableschema.hpp"
#include "qkdbfield.hpp"
#include "qkdbrow.hpp"
#include "qkdbquery.hpp"

class QkDb;

class QKDB_EXPORT QkDbTableBase : public QkThrowable
{
    Q_DISABLE_COPY(QkDbTableBase)

public:
    QkDbTableBase(QkDb* pDb, const QString& pName, const QString& pTitle, const QString& pDescr, QkDbField* pJoinParentField);
    virtual ~QkDbTableBase();

public:
    const QkDbTableSchema* schema() const { return &mSchema; }
    QkDb* db() const { return mDb; }

public:
    QkDbRow selectRow(QVariant pID) const;
    QkDbRow newRow() const { return createRow(); }
    QkDbQuery select() const;
    virtual bool init();
    virtual bool migrate();

protected:
    virtual QkDbRow createRow() const = 0;

private:
    QkDb* mDb = nullptr;
    QkDbTableSchema mSchema;
};


template<class TRowType = QkDbRow>
class QkDbTable : public QkDbTableBase
{
public:
    QkDbTable(QkDb* pDb, const QString& pName, const QString& pTitle, const QString& pDescr, QkDbField* pJoinParentField)
        : QkDbTableBase(pDb, pName, pTitle, pDescr, pJoinParentField)
    {
    }

public:
    TRowType selectRow(QVariant pID) const;
    TRowType newRow() const { return TRowType(this); }

protected:
    virtual QkDbRow createRow() const { return newRow(); }
};
