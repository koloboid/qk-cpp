#include "core.export.hpp"
#include "enumitem.hpp"

#ifdef ENUMIMPL

#undef EnumBegin
#undef Enum
#undef EnumEnd


#define EnumBegin(pName, pBaseType, pDefault) \
    template<> QMap<pBaseType, Qk::Core::EnumDesc<pName>*> Qk::Core::EnumItem<pName, pBaseType>::mDescMap = QMap<pBaseType, Qk::Core::EnumDesc<pName>*>();

#define Enum(pType, pName, pValue, pTitle, pDescr) constexpr pType::BaseType pType::pName; \
    Qk::Core::EnumDesc<pType> pType::__desc##pName(pValue, #pName, pTitle, pDescr);

#define EnumEnd

#else

#undef EnumBegin
#undef Enum
#undef EnumEnd


#define EnumBegin(pName, pBaseType, pDefault) class pName : public Qk::Core::EnumItem<pName, pBaseType> \
    { public: pName() : EnumItem(pDefault) { } \
    pName(BaseType pVal) : EnumItem(pVal) { }

#define Enum(pType, pName, pValue, pTitle, pDescr) public: static constexpr BaseType pName = BaseType(pValue); \
bool is##pName() const { return mValue == pValue; }; \
bool has##pName() const { return (mValue & pValue) != 0; }; \
void set##pName() { mValue = mValue | pName; } \
void reset##pName() { mValue = mValue | pName; } \
private: static Qk::Core::EnumDesc<pType> __desc##pName;

#define EnumEnd };

#endif
