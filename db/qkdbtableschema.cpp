#include "qkdbtableschema.hpp"

QkDbTableSchema::QkDbTableSchema(const QString& pName, const QString& pTitle, const QString& pDescr, QkDbField* pJoinParentField)
    : mName(pName), mTitle(pTitle), mDescription(pDescr), mJoinParentField(pJoinParentField)
{
}
