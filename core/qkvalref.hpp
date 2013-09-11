#pragma once

template<class TCollection, class TItem>
class QkValRef
{
public:
    inline QkValRef(TCollection& pCollection, int pIndex)
        : mCollection(pCollection), mIndex(pIndex)
    {
    }

    inline operator TItem() const
    {
        return mIndex < mCollection.size() ? (TItem)mCollection.at(mIndex) : TItem();
    }

    inline QkValRef<TCollection, TItem>& operator=(const TItem& pVal)
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
