#pragma once

#include <QVariant>
#include <QSet>
#include <QSqlRecord>
#include <QExplicitlySharedDataPointer>

#include "qkdb.export.hpp"
#include "qkdbvalue.hpp"
#include "qkdbrowstate.hpp"
#include <qktimespan.hpp>
#include <qkthrowable.hpp>
#include <qkdbdriver.hpp>
#include <qkfuture.hpp>

class QkDbTableBase;
class QkDbField;

class QKDB_EXPORT QkDbRow : public QkThrowable
{
public:
    static QMetaType::Type metaTypeID();

public:
    QkDbRow();
    QkDbRow(const QkDbTableBase* pTable);
    QkDbRow(const QkDbTableBase* pTable, QString pJSON);
    QkDbRow(const QkDbTableBase* pTable, QSqlRecord pRecord);
    virtual ~QkDbRow();

public:
    const QkDbTableBase* table() const { return mData->mTable; }
    QString toString() const;
    QString toJSON() const;
    QkDbRowState state() const;
    QSet<const QkDbField*> changes() const;
    QList<QkError> check() const;

    QkFuture<bool> save(const QkFuture<bool>::StdCallback& pCallback = QkFuture<bool>::StdCallback());
    void scheduleSave(QkTimeSpan pTime = 5000);
    QkFuture<bool> drop(const QkFuture<bool>::StdCallback& pCallback = QkFuture<bool>::StdCallback());
    QkFuture<bool> reload(const QkFuture<bool>::StdCallback& pCallback = QkFuture<bool>::StdCallback());

    QVariant get(const QkDbField& pField) const;
    QVariant get(const QkDbField* pField) const;
    void set(const QkDbField& pField, QVariant pValue);
    void set(const QkDbField* pField, QVariant pValue);

private:
    class RowData : public QSharedData
    {
    public:
        QkDbRowState mState;
        QMap<const QkDbField*, QVariant> mValues;
        QMap<const QkDbField*, QkDbRow> mLinkedRows;
        QSet<const QkDbField*> mChangedFields;
        bool mNeedWriteToHistory = false;
        const QkDbTableBase* mTable;
        bool mLocked = false;

        RowData(const QkDbTableBase* pTable, QkDbRowState pState = QkDbRowState::New)
            : mState(pState), mTable(pTable)
        {
        }
    };
    QExplicitlySharedDataPointer<RowData> mData;
};

Q_DECLARE_METATYPE(QkDbRow)
