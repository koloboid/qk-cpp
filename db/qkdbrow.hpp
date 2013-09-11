#pragma once

#include <QVariant>
#include <QSet>
#include <QSqlRecord>
#include <QExplicitlySharedDataPointer>

#include "qkdb.export.hpp"
#include "qkdbvalue.hpp"
#include "qkdbrowstate.hpp"
#include <qkthrowable.hpp>

class QkDbTableBase;
class QkDbField;
typedef quint64 QkDbRowID;

class QKDB_EXPORT QkDbRow : public QkThrowable
{
public:
    QkDbRow(const QkDbTableBase* pTable);
    QkDbRow(const QkDbTableBase* pTable, QString pJSON);
    QkDbRow(const QkDbTableBase* pTable, QSqlRecord pRecord);
    virtual ~QkDbRow();

public:
    const QkDbTableBase* table() const { return mData->mTable; }
    QkDbRowID rowId() const;
    QString toString() const;
    QString toJSON() const;
    bool isOriginal() const;
    bool isNew() const;
    bool isModified() const;
    QkDbRowState state() const;
    QSet<const QkDbField*> changes() const;
    QList<QkError> check() const;

    bool save();
    void scheduleSave();
    bool drop();
    bool reload();

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

        RowData(const QkDbTableBase* pTable, QkDbRowState pState = QkDbRowState::New)
            : mState(pState), mTable(pTable)
        {
        }
    };
    QExplicitlySharedDataPointer<RowData> mData;
};
