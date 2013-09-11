#include "qkdbcondition.hpp"

class QkDbConditionData : public QSharedData
{
public:
   QkDbCondition::EOperator mOperator;
   QkDbCondition::EType mType;
   const QkDbField* mFieldLeft;
   const QkDbField* mFieldRight;
   QVariant mValueLeft;
   QVariant mValueRight;
   QkDbCondition mConditionLeft;
   QkDbCondition mConditionRight;
};

QkDbCondition::QkDbCondition()
{
}
