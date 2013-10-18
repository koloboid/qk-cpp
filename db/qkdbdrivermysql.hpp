#pragma once

#include "qkdbdriver.hpp"
#include <QSqlDatabase>

class QKDB_EXPORT QkDbDriverMySql : public QkDbDriver
{
public:
    QkDbDriverMySql(const QString& pHost, quint16 pPort, const QString& pDbName, const QString& pUserName,
                    const QString& pPassword, const QMap<QString, QString>& pParams);
    virtual ~QkDbDriverMySql();

public:
    virtual bool connect();
    virtual bool disconnect();

    virtual bool transaction();
    virtual bool commit();
    virtual bool rollback();


    virtual void insertRow(const QkDbRow& pRow);
    virtual void updateRow(const QkDbRow& pRow);
    virtual void deleteRow(const QkDbRow& pRow);


    virtual void migrateDb(const QkDb* pSchema);
    virtual void migrateTable(const QkDbTableBase* pTable);
    virtual void migrateField(const QkDbField* pField);
    virtual void migrateIndex(const QkDbIndex* pIndex);

private:
    QSqlDatabase mDb;
};
