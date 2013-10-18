#pragma once

#include "qkdb.export.hpp"
#include <qkthrowable.hpp>
#include "qkdbdriver.hpp"

#include <QMap>
#include <qkfuture.hpp>

class QkDbTableBase;
class QkConfig;

class QKDB_EXPORT QkDb : public QkThrowable
{
    Q_DISABLE_COPY(QkDb)
    friend class QkDbDriverAllocator;

public:
    QkDb();
    virtual ~QkDb();

public:
    template<class TTable>
    TTable* registerTable() { return (TTable*)addTable(new TTable(this)); }
    template<class TTable = QkDbTableBase>
    TTable* getTable(const QString& pName) const { return (TTable*)mTables[pName]; }
    template<class TTable>
    TTable* getTable() const { return (TTable*)mTables[TTable::tableName]; }

    virtual bool init();
    virtual bool setupConnection(const QString& pConnectionString);

    virtual QkFuture<bool> migrate(const QkFuture<bool>::StdCallback& pCallback = QkFuture<bool>::StdCallback(0));
    virtual QkFuture<bool> exec(std::function<void(QkDbDriver*)> pCode);

private:
    QkDbTableBase* addTable(QkDbTableBase* pTable);

private:
    QMap<QString, QkDbTableBase*> mTables;
    QString mConnectionString;
    QMap<QString, QString> mConnectionOptions;
    QString mDriverName;
    QString mHostName;
    QString mDbName;
    QString mUserName;
    QString mPassword;
    quint32 mPoolSize = 0;
    quint16 mPort = 0;
};
