#pragma once

#include "qkdb.export.hpp"
#include "qkobjectpool.hpp"
#include <QThread>

class QkDb;
class QkDbTableBase;
class QkDbField;
class QkDbIndex;
class QkDbRow;
class QkDbDriver;

class QKDB_EXPORT QkDbDriver : private QThread, public QkPooledObject
{
public:
    QkDbDriver();
    virtual ~QkDbDriver();

public:
    virtual bool connect() = 0;
    virtual bool disconnect() = 0;

    virtual bool transaction() = 0;
    virtual bool commit() = 0;
    virtual bool rollback() = 0;


    virtual void insertRow(const QkDbRow& pRow) = 0;
    virtual void updateRow(const QkDbRow& pRow) = 0;
    virtual void deleteRow(const QkDbRow& pRow) = 0;


    virtual void migrateDb(const QkDb* pSchema) = 0;
    virtual void migrateTable(const QkDbTableBase* pTable) = 0;
    virtual void migrateField(const QkDbField* pField) = 0;
    virtual void migrateIndex(const QkDbIndex* pIndex) = 0;

protected:
    virtual void run() override;
};
