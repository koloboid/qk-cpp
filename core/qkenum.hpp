#include "qkcore.export.hpp"
#include "qkenumitem.hpp"

#include <QMap>
#include <QString>
#include <QCoreApplication>

#ifdef ENUMIMPL

#undef QkEnumBegin
#undef QkEnum
#undef QkEnumEnd

#define QkEnumBegin(pName, pType) QMap<pName::EValue, QkEnumItem> pName::mItems;\
    void pName::fillItems() {

#define QkEnum(pName, pValue, pTitle, pDescription) mItems.insert(pName, QkEnumItem(pValue, #pName,  qApp->translate("", pTitle), qApp->translate("", pDescription)));

#define QkEnumEnd(pName, pDefaultValue) }\
    QAbstractItemModel* pName::model() \
    { \
        checkItems(); \
        return nullptr; \
    }

#else

#undef QkEnumBegin
#undef QkEnum
#undef QkEnumEnd

#define QkEnumBegin(pName, pType) class pName { public: enum EValue : pType

#define QkEnum(pName, pValue, pTitle, pDescription) pName = pValue,

#define QkEnumEnd(pName, pDefaultValue) ; \
  public:\
    pName() : mValue(pDefaultValue) { }\
    pName(EValue pValue) : mValue(pValue) { }\
    explicit pName(quint64 pValue) : mValue((EValue)pValue) { }\
    \
    QString name() const { checkItems(); return mItems[mValue].name(); }\
    QString title() const { checkItems(); return mItems[mValue].title(); }\
    QString description() const { checkItems(); return mItems[mValue].description(); } \
    static QMap<EValue, QkEnumItem> items() { checkItems(); return mItems; }; \
    static QAbstractItemModel* model(); \
    static EValue inverse(EValue pVal) { return (EValue)~pVal; } \
    bool isSet(EValue pValue) const { return mValue & pValue; } \
    bool isNotSet(EValue pValue) const { return !(mValue & pValue); } \
    quint64 asInt() const { return mValue; } \
    operator EValue() const { return mValue; } \
  private: \
    static void checkItems() { if (mItems.size() == 0) fillItems(); } \
    static void fillItems(); \
    static QMap<EValue, QkEnumItem> mItems; \
    EValue mValue; };

#endif
