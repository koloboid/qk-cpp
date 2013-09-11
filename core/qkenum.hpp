#include "qkcore.export.hpp"
#include "qkenumitem.hpp"

#ifdef ENUMIMPL

#undef QkEnumBegin
#undef QkEnum
#undef QkEnumEnd


#define QkEnumBegin(pName, pBaseType, pDefault) \
    template<> QMap<pBaseType, QkEnumDesc<pName>*> QkEnumItem<pName, pBaseType>::mDescMap = QMap<pBaseType, QkEnumDesc<pName>*>();

#define QkEnum(pType, pName, pValue, pTitle, pDescr) constexpr pType::BaseType pType::pName; \
    QkEnumDesc<pType> pType::__desc##pName(pValue, #pName, pTitle, pDescr);

#define QkEnumEnd

#else

#undef QkEnumBegin
#undef QkEnum
#undef QkEnumEnd


#define QkEnumBegin(pName, pBaseType, pDefault) class pName : public QkEnumItem<pName, pBaseType> \
{ public: pName() : QkEnumItem(pDefault) { } \
pName(BaseType pVal) : QkEnumItem(pVal) { }

#define QkEnum(pType, pName, pValue, pTitle, pDescr) public: static constexpr BaseType pName = BaseType(pValue); \
bool is##pName() const; \
bool has##pName() const; \
void set##pName() { mValue = mValue | pName; } \
void reset##pName() { mValue = mValue | pName; } \
private: static QkEnumDesc<pType> __desc##pName;

#define QkEnumEnd };

#endif
