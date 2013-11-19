#pragma once

namespace Qk
{
namespace Core
{

template<class TCollection, class TItem>
class ValRef
{
public:
    inline ValRef(TCollection& pCollection, int pIndex)
        : mCollection(pCollection), mIndex(pIndex)
    {
    }

    inline operator TItem() const
    {
        return mIndex < mCollection.size() ? (TItem)mCollection.at(mIndex) : TItem();
    }

    inline ValRef<TCollection, TItem>& operator=(const TItem& pVal)
    {
        if (mIndex < mCollection.size())
        {
            mCollection[mIndex] = pVal;
        }
        return *this;
    }

private:
    TCollection& mCollection;
    int mIndex;
};

}
}
