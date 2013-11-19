#pragma once

#define QkField(pType, pName, pValueName) \
    public: static Field<pType> pName; \
    Value<pType> pValueName;

#define QkFieldRO(pMyClass, pType, pName, pValueName) \
    public: static Field<pType> pName; \
    ValueRO<pType, pMyClass> pValueName;

#define QkFieldImpl(pClass, pType, pFieldName, pValueName) \
    Field<pType> pClass::pFieldName = Field<pType>(#pFieldName, &pClass::TableSchema, &(((pClass*)0)->pValueName));

#define QkFieldImplBase(pBaseClass, pClass, pType, pFieldName, pValueName) \
    template<> Field<pType> pBaseClass<pClass>::pFieldName = Field<pType>(#pFieldName, &pClass::TableSchema, &(((pClass*)0)->pValueName));

#define QkTable(pFullClassName, pName) \
    template<> Qk::Db::Table<pFullClassName> Qk::Db::Model<pFullClassName>::TableSchema = Qk::Db::Table<pFullClassName>(pName);
