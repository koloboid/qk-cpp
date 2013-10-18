#include "qkdbfield.hpp"

QkDbField::QkDbField(QkDbTableBase *pTable, QMetaType::Type pType, QString pName, QString pTitle, QString pDescription,
                     QVariant pDefaultValue, QkDbFieldFlag pFlag)
    : mTable(pTable), mType(pType), mName(pName), mTitle(pTitle), mDescription(pDescription), mDefaultValue(pDefaultValue),
      mFlag(pFlag)
{
}

QkDbField::~QkDbField()
{
}

bool QkDbField::link()
{
    return throwNotImplemented(ERRLOC);
}

bool QkDbField::checkValue(QVariant pValue) const
{
    if (!pValue.canConvert(mType))
    {
        return throwNow(ERRLOC, tr("Тип значения '%1' для поля '%2' невозможно преобразовать в тип '%3'")
                        .arg(pValue.typeName())
                        .arg(mName)
                        .arg(mType));
    }
    return false;
}

bool QkDbFieldString::checkValue(QVariant pValue) const
{
    if (!this->assertStr(ERRLOC, pValue, mMinSize, mMaxSize, name())) return false;
    return true;
}

bool QkDbFieldDateTime::checkValue(QVariant pValue) const
{
    if (!this->assertVal(ERRLOC, pValue, mMinValue, mMaxValue, name())) return false;
    return true;
}
