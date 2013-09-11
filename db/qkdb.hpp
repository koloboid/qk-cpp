#pragma once

#include "qkdb.export.hpp"
#include <qkthrowable.hpp>
#include "qkdbtable.hpp"

#include <QMap>

class QkDbTableBase;

class QKDB_EXPORT QkDb : public QkThrowable
{
    Q_DISABLE_COPY(QkDb)

public:
    QkDb();
    virtual ~QkDb();

public:
    template<class TTable>
    TTable* createTable() { return addTable(new TTable(this)); }
    template<class TTable = QkDbTableBase>
    TTable* getTable(const QString& pName) const { return (TTable*)mTables[pName]; }
    virtual void init();

private:
    QkDbTableBase* addTable(QkDbTableBase* pTable);

private:
    QMap<QString, QkDbTableBase*> mTables;
};
