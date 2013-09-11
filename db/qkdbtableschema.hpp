#pragma once

#include "qkdb.export.hpp"
#include "qkdbfield.hpp"
#include <qkthrowable.hpp>
#include <QMap>

class QKDB_EXPORT QkDbTableSchema : public QkThrowable
{
    Q_DISABLE_COPY(QkDbTableSchema)

public:
    QkDbTableSchema(const QString& pName, const QString& pTitle, const QString& pDescr, QkDbField* pJoinParentField);

public:
    QString name() const { return mName; }
    QString title() const { return mTitle; }
    QString description() const { return mDescription; }
    QMap<QString, QkDbField*> fields() const { return mFields; }
    QkDbField* field(const QString& pName) { return mFields[pName]; }
    bool addField(QkDbField* pField);

    QkDbField* operator[](const QString& pName) const;

private:
    QString mName;
    QString mTitle;
    QString mDescription;
    QkDbField* mPrimaryField = nullptr;
    QkDbField* mDisplayField = nullptr;
    QkDbField* mJoinParentField = nullptr;
    QMap<QString, QkDbField*> mFields;
};
