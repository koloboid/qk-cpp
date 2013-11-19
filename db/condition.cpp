#include <qk/core/error.hpp>
#include "condition.hpp"
#include "field.hpp"

namespace Qk
{
namespace Db
{

class ConditionData : public QSharedData
{
public:
   Condition::EOperator mOperator;
   Condition::EType mType;
   const IField* mFieldLeft;
   const IField* mFieldRight;
   QVariant mValueLeft;
   QVariant mValueRight;
   Condition mConditionLeft;
   Condition mConditionRight;
};

QString operatorToString(Condition::EOperator pOperator)
{
    switch (pOperator)
    {
        case Condition::OpAnd:
            return " && ";
        case Condition::OpOr:
            return " || ";
        case Condition::OpEqual:
            return " == ";
        case Condition::OpGreater:
            return " > ";
        case Condition::OpGreaterOrEqual:
            return " >= ";
        case Condition::OpLess:
            return " < ";
        case Condition::OpLessOrEqual:
            return " <= ";
        case Condition::OpLike:
            return  " % ";
        default:
            return "UNKNOWN";
    }
}

Condition::Condition()
{
}

Condition::Condition(const Condition& pCondition)
    : mData(pCondition.mData)
{
}

Condition::~Condition()
{
}

Condition::Condition(const IField* pFieldLeft, EOperator pOperator, QVariant pValueRight)
{
    mData = new ConditionData();
    mData->mFieldLeft = pFieldLeft;
    mData->mValueRight = pValueRight;
    mData->mOperator = pOperator;
    mData->mType = TypeFieldValue;
}

Condition::Condition(const Condition& pCondLeft, EOperator pOperator, const Condition& pCondRight)
{
    mData = new ConditionData();
    mData->mConditionLeft = pCondLeft;
    mData->mConditionRight = pCondRight;
    mData->mOperator = pOperator;
    mData->mType = TypeConditionCondition;
}

Condition Condition::conditionLeft() const
{
    if (!mData) return Condition();
    return mData->mConditionLeft;
}

Condition Condition::conditionRight() const
{
    if (!mData) return Condition();
    return mData->mConditionRight;
}

const IField* Condition::fieldLeft() const
{
    if (!mData) return nullptr;
    return mData->mFieldLeft;
}

const IField* Condition::fieldRight() const
{
    if (!mData) return nullptr;
    return mData->mFieldRight;
}

Condition::EOperator Condition::op() const
{
    if (!mData) return OpAnd;
    return mData->mOperator;
}

Condition::EType Condition::type() const
{
    if (!mData) return TypeInvalid;
    return mData->mType;
}

QVariant Condition::valueLeft() const
{
    if (!mData) return QVariant();
    return mData->mValueLeft;
}

QVariant Condition::valueRight() const
{
    if (!mData) return QVariant();
    return mData->mValueRight;
}

QString Condition::toString(const Condition& pCond, QString pText)
{
    if (pCond.type() == TypeInvalid) return pText;
    switch (pCond.type())
    {
        case TypeConditionCondition:
            pText += toString(pCond.conditionLeft()) + operatorToString(pCond.op()) + toString(pCond.conditionRight());
            break;
        case TypeFieldValue:
            pText += pCond.fieldLeft()->name() + operatorToString(pCond.op()) + "\"" + pCond.valueRight().toString() + "\"";
            break;
        default:
            throw Qk::Core::ErrorNotImplemented(ERRLOC);
    }
    return pText;
}

Condition& Condition::operator=(const Condition& pCond)
{
    mData = pCond.mData;
    return *this;
}

Condition Condition::operator&&(const Condition& pCond) const
{
    return Condition(*this, Condition::OpAnd, pCond);
}

Condition Condition::operator||(const Condition& pCond) const
{
    return Condition(*this, Condition::OpOr, pCond);
}

}
}
