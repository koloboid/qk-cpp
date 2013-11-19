#include "field.hpp"
#include "value.hpp"
#include "model.hpp"

namespace Qk {
namespace Db {

void IField::init()
{
}

QVariant IField::get(const IModel* pModel) const
{
    IValue* val = (IValue*)((quint8*)pModel + mOffset);
    return val->mValue;
}

void IField::set(const IModel* pModel, QVariant pVal) const
{
    if (linkedTo() && pVal.type() == linkedTo()->primaryField()->type())
    {
        pVal = linkedTo()->newLinkRow(pVal);
    }
    IValue* val = (IValue*)((quint8*)pModel + mOffset);
    val->mValue = pVal;
}

}
}
