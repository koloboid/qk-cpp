#pragma once

#include <QString>
#include <QAbstractItemModel>

class QkEnumItem
{
public:
    QkEnumItem()
        : mValue(0)
    {
    }

    QkEnumItem(quint64 pValue, const QString& pName, const QString& pTitle, const QString& pDescription)
        : mValue(pValue), mName(pName), mTitle(pTitle), mDescription(pDescription)
    {
    }

public:
    quint64 value() const { return mValue; }
    QString name() const { return mName; }
    QString title() const { return mTitle; }
    QString description() const { return mDescription; }

private:
    quint64 mValue;
    QString mName;
    QString mTitle;
    QString mDescription;
};
