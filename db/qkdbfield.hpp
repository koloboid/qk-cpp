#pragma once

#include <QDateTime>
#include <qkerror.hpp>
#include <qkblob.hpp>
#include "qkdbrow.hpp"
#include "qkdbcondition.hpp"
#include "qkdbfieldflag.hpp"
#include "qktimespan.hpp"
#include "qkdb.hpp"

class QkDbTableBase;
class QkDb;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbField : public QkThrowable
{
    Q_DISABLE_COPY(QkDbField)

public:
    QkDbField(QkDbTableBase* pTable, QMetaType::Type pType, QString pName, QString pTitle, QString pDescription,
              QVariant pDefaultValue, QkDbFieldFlag pFlag);
    virtual ~QkDbField();

    virtual bool link();
    virtual bool checkValue(QVariant pValue) const;

    QVariant get(const QkDbRow& pRow) const { return pRow.get(this); }
    void set(QkDbRow& pRow, QVariant pValue) const { pRow.set(this, pValue); }

    QString title() const { return mTitle; }
    QString name() const { return mName; }
    QString description() const { return mDescription; }

public:
    QkDbCondition operator==(QVariant pValue) const { return QkDbCondition(*this, QkDbCondition::OpEqual, pValue); }
    QkDbCondition operator%(QString pString) const { return QkDbCondition(*this, QkDbCondition::OpLike, pString); }
    QVariant operator()(const QkDbRow& pRow) const { return get(pRow); }
    void operator()(QkDbRow& pRow, QVariant pValue) const { set(pRow, pValue); }

private:
    const QkDbTableBase* mTable;
    const QMetaType::Type mType;
    const QString mName;
    const QString mTitle;
    const QString mDescription;
    const QVariant mDefaultValue;
    const QkDbFieldFlag mFlag;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TType>
class QkDbFieldGeneric : public QkDbField
{
public:
    QkDbFieldGeneric(QkDbTableBase* pTable, QMetaType::Type pType, QString pName, QString pTitle = QString(), QString pDescription = QString(),
                   TType pDefaultValue = QVariant(), QkDbFieldFlag pFlag = QkDbFieldFlag::Simple)
        : QkDbField(pTable, pType, pName, pTitle, pDescription, QVariant::fromValue<TType>(pDefaultValue), pFlag)
    {
    }

public:
    TType get(const QkDbRow& pRow) const { return QkDbField::get(pRow).value(); }
    void set(QkDbRow& pRow, TType pValue) const { QkDbField::set(pRow, pValue); }

    TType operator()(const QkDbRow& pRow) const { return get(pRow); }
    void operator()(QkDbRow& pRow, TType pValue) const { set(pRow, pValue); }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldString : public QkDbField
{
public:
    QkDbFieldString(QkDbTableBase* pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(), quint32 pMinSize = 0,
                  quint32 pMaxSize = 256, QString pDefault = QString(), QkDbFieldFlag pFlag = QkDbFieldFlag::Simple | QkDbFieldFlag::Filterable)
        : QkDbField(pTable, QMetaType::QString, pName, pTitle, pDescription, pDefault, pFlag),
          mMinSize(pMinSize), mMaxSize(pMaxSize)
    {
    }

public:
    virtual bool checkValue(QVariant pValue) const override;
    quint32 minSize() const { return mMinSize; }
    quint32 maxSize() const { return mMaxSize; }

    QString get(const QkDbRow& pRow) const { return QkDbField::get(pRow).toString(); }
    void set(QkDbRow& pRow, QString pValue) const { QkDbField::set(pRow, pValue); }

    QString operator()(const QkDbRow& pRow) const { return get(pRow); }
    void operator()(QkDbRow& pRow, QString pValue) const { set(pRow, pValue); }

private:
    quint32 mMinSize;
    quint32 mMaxSize;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldURL : public QkDbFieldString
{
public:
    QkDbFieldURL(QkDbTableBase* pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(),
               QString pDefault = QString(), QkDbFieldFlag pFlag = QkDbFieldFlag::Simple)
        : QkDbFieldString(pTable, pName, pTitle, pDescription, 0, 256, pDefault, pFlag)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldImage : public QkDbFieldString
{
public:
    QkDbFieldImage(QkDbTableBase* pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(),
                 QString pDefault = QString(), QkDbFieldFlag pFlag = QkDbFieldFlag::Simple)
        : QkDbFieldString(pTable, pName, pTitle, pDescription, 0, 256, pDefault, pFlag)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldDescription : public QkDbFieldString
{
public:
    QkDbFieldDescription(QkDbTableBase* pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(),
                       QString pDefault = QString(), QkDbFieldFlag pFlag = (QkDbFieldFlag)(QkDbFieldFlag::Simple | QkDbFieldFlag::Filterable))
        : QkDbFieldString(pTable, pName, pTitle, pDescription, 0, 4096, pDefault, pFlag)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldBool : public QkDbFieldGeneric<bool>
{
public:
    QkDbFieldBool(QkDbTableBase* pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(),
                bool pDefault = false, QkDbFieldFlag pFlag = QkDbFieldFlag::Simple)
        : QkDbFieldGeneric<bool>(pTable, QMetaType::Type::Bool, pName, pTitle, pDescription, pDefault, pFlag)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldTimeSpan : public QkDbFieldGeneric<QkTimeSpan>
{
public:
    QkDbFieldTimeSpan(QkDbTableBase* pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(), QkTimeSpan pMin = QkTimeSpan::minValue,
                    QkTimeSpan pMax = QkTimeSpan::maxValue, QkTimeSpan pDefault = QkTimeSpan::minValue, QkDbFieldFlag pFlag = QkDbFieldFlag::Simple)
        : QkDbFieldGeneric<QkTimeSpan>(pTable, QkTimeSpan::metaTypeID(), pName, pTitle, pDescription, pDefault, pFlag),
          mMinValue(pMin), mMaxValue(pMax)
    {
    }

public:
    virtual bool checkValue(QVariant pValue) const override;
    QkTimeSpan MinValue() const { return mMinValue; }
    QkTimeSpan MaxValue() const { return mMaxValue; }

private:
    QkTimeSpan mMinValue;
    QkTimeSpan mMaxValue;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TRefTable>
class QkDbFieldLink : public QkDbFieldGeneric<QkDbRow>
{
public:
    QkDbFieldLink(QkDbTableBase* pTable, const QkDb* pDb, QString pName, QString pTitle = QString(), QString pDescription = QString(), QkDbFieldFlag pFlag = QkDbFieldFlag::RefStrong)
        : QkDbFieldGeneric<QkDbRow>(pTable, QkDbRow::metaTypeID(), pName, pTitle, pDescription, QkDbRow(pTable), pFlag),
          Join(pDb->getTable<TRefTable>(""))
    {
    }

public:
    const TRefTable* Join;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TEnum, QMetaType::Type TBasetype>
class QkDbFieldEnum : public QkDbField
{
public:
    QkDbFieldEnum(QkDbTableBase* pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(), QkDbFieldFlag pFlag = QkDbFieldFlag::Simple)
        : QkDbField(pTable, (QMetaType::Type)TBasetype, pName, pTitle, pDescription, 0, pFlag)
    {
    }

public:
    TEnum get(const QkDbRow& pRow) const
    {
        return TEnum(QkDbField::get(pRow).AsUInt64());
    }
    void set(QkDbRow& pRow, const TEnum& pValue) const
    {
        QkDbField::set(pRow, pValue.AsInt());
    }
    TEnum operator()(const QkDbRow& pRow) const
    {
        return get(pRow);
    }
    void operator()(QkDbRow& pRow, const TEnum& pValue) const
    {
        QkDbField::set(pRow, pValue.AsInt());
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldBlob : public QkDbFieldGeneric<QkBlob>
{
public:
    QkDbFieldBlob(QkDbTableBase* pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(), quint32 pMinSize = 0,
                quint32 pMaxSize = 1024, QkBlob pDefault = QkBlob(), QkDbFieldFlag pFlag = QkDbFieldFlag::Simple)
        : QkDbFieldGeneric<QkBlob>(pTable, QMetaType::QByteArray, pName, pTitle, pDescription, pDefault, pFlag),
          mMinSize(pMinSize), mMaxSize(pMaxSize)
    {
    }

public:
    virtual bool checkValue(QVariant pValue) const override;
    quint32 minSize() const { return mMinSize; }
    quint32 maxSize() const { return mMaxSize; }

private:
    quint32 mMinSize;
    quint32 mMaxSize;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QkDbFieldDateTime : public QkDbFieldGeneric<QDateTime>
{
public:
    QkDbFieldDateTime(QkDbTableBase* pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(),
                    QDateTime pMin = QDateTime::fromMSecsSinceEpoch(std::numeric_limits<qint64>::min()),
                    QDateTime pMax = QDateTime::fromMSecsSinceEpoch(std::numeric_limits<qint64>::max()),
                    QDateTime pDefault = QDateTime::currentDateTime(), QkDbFieldFlag pFlag = QkDbFieldFlag::Simple)
        : QkDbFieldGeneric<QDateTime>(pTable, QMetaType::QDateTime, pName, pTitle, pDescription, pDefault, pFlag),
          mMinValue(pMin), mMaxValue(pMax)
    {
    }

public:
    virtual bool checkValue(QVariant pValue) const override;
    QDateTime minValue() const { return mMinValue; }
    QDateTime maxValue() const { return mMaxValue; }

private:
    QDateTime mMinValue;
    QDateTime mMaxValue;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename TNumber, QMetaType::Type TType>
class QkDbFieldNumber : public QkDbFieldGeneric<TNumber>
{
public:
    QkDbFieldNumber(QkDbTableBase* pTable, QString pName, QString pTitle = QString(), QString pDescription = QString(),
                  TNumber pMin = std::numeric_limits<TNumber>::min(),
                  TNumber pMax = std::numeric_limits<TNumber>::max(),
                  TNumber pDefault = 0, QkDbFieldFlag pFlag = QkDbFieldFlag::Simple)
        : QkDbFieldGeneric<TNumber>(pTable, (QMetaType::Type)TType, pName, pTitle, pDescription, pDefault, pFlag),
          mMinValue(pMin), mMaxValue(pMax)
    {
    }

public:
    virtual bool checkValue(QVariant pValue) const override
    {
        TNumber num = pValue.value<TNumber>();
        if (num < mMinValue)
            return this->throwNow(QkError(ERRLOC, QCoreApplication::translate("", "Поле '%1' должно быть больше чем '%2'").arg(this->title()).arg(mMinValue)));
        if (num > mMaxValue)
            if (num > mMaxValue)
            return this->throwNow(QkError(ERRLOC, QCoreApplication::translate("", "Поле '%1' должно быть не более чем '%2'").arg(this->title()).arg(mMaxValue)));
        return QkDbFieldGeneric<TNumber>::checkValue(pValue);
    }

    TNumber minValue() const { return mMinValue; }
    TNumber maxValue() const { return mMaxValue; }

private:
    TNumber mMinValue;
    TNumber mMaxValue;
};

typedef QkDbFieldNumber<quint8, QMetaType::UChar> QkDbFieldUInt8;
typedef QkDbFieldNumber<quint16, QMetaType::UShort> QkDbFieldUInt16;
typedef QkDbFieldNumber<quint32, QMetaType::UInt> QkDbFieldUInt32;
typedef QkDbFieldNumber<quint64, QMetaType::ULongLong> QkDbFieldUInt64;

typedef QkDbFieldNumber<qint8, QMetaType::Char> QkDbFieldInt8;
typedef QkDbFieldNumber<qint16, QMetaType::Short> QkDbFieldInt16;
typedef QkDbFieldNumber<qint32, QMetaType::Int> QkDbFieldInt32;
typedef QkDbFieldNumber<qint64, QMetaType::LongLong> QkDbFieldInt64;

typedef QkDbFieldNumber<float, QMetaType::Float> QkDbFieldFloat;
typedef QkDbFieldNumber<double, QMetaType::Double> QkDbFieldDouble;
