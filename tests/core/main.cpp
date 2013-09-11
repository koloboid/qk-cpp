#include <QCoreApplication>
#include <qklog.hpp>

void fun()
{
    QkLogScope("");
}

class EnumDesc
{
public:
    EnumDesc(QString pName, QString pDesc)
    {
    }

    EnumDesc()
    {
    }

    QString Name;
    QString Desc;
};

template<typename TEnum, class TBase>
class Enum
{
public:
    class BaseType
    {
        friend TEnum;

    private:
        constexpr BaseType(TBase pValue)
            : mValue(pValue)
        {
        }

    public:
        constexpr operator TBase() const
        {
            return mValue;
        }

        explicit operator TEnum() const
        {
            return TEnum(mValue);
        }

        TEnum operator()() const
        {
            return TEnum(mValue);
        }

        TEnum getEnum() const
        {
            return TEnum(mValue);
        }

        BaseType operator|(const BaseType& pVal) const
        {
            return mValue | pVal.mValue;
        }

    private:
        TBase mValue;
    };

    Enum(BaseType pVal)
        : mValue(pVal)
    {
    }

    operator TBase() const
    {
        return mValue;
    }

    QString getName() const { return mDescMap[mValue].Name; }
    QString getDesc() const { return mDescMap[mValue].Desc; }

protected:
    BaseType mValue;
    static QMap<TBase, EnumDesc> mDescMap;
};

class MyEnum : public Enum<MyEnum, quint32>
{
public:
    static constexpr BaseType Foo = BaseType(5);
    static constexpr BaseType Bar = BaseType(7);

    QString getName() const { return mDescMap[mValue].Name; }
    QString getDesc() const { return mDescMap[mValue].Desc; }

    bool isFoo() const;
    bool isBar() const;

    void setFoo() { mValue = mValue | Foo; }
    void setBar() { mValue = mValue | Bar; }

    void resetFoo() { mValue = mValue | Foo; }
    void resetBar() { mValue = mValue | Bar; }

    MyEnum()
        : Enum(Foo)
    {
    }

    MyEnum(BaseType pVal)
        : Enum(pVal)
    {
    }

private:
    static EnumDesc descFoo;
};

class OtherEnum : public Enum<OtherEnum, quint32>
{
public:
    static constexpr BaseType Foo = BaseType(5);
    static constexpr BaseType Bar = BaseType(7);

    QString getName() const { return mDescMap[mValue].Name; }
    QString getDesc() const { return mDescMap[mValue].Desc; }

    bool isFoo() const;
    bool isBar() const;

    void setFoo() { mValue = mValue | Foo; }
    void setBar() { mValue = mValue | Bar; }

    void resetFoo() { mValue = mValue | Foo; }
    void resetBar() { mValue = mValue | Bar; }

    OtherEnum()
        : Enum(Foo)
    {
    }

    OtherEnum(BaseType pVal)
        : Enum(pVal)
    {
    }

private:
    static EnumDesc descFoo;
};

#define QKEnum(pName, pBaseType, pDefault) class pName : public Enum<pName, pBaseType> \
{ public: pName() : Enum(pDefault) { } \
pName(BaseType pVal) : Enum(pVal) { }

#define QKEnumItem(pName, pValue, pTitle, pDescr) public: static constexpr BaseType pName = BaseType(pValue); \
bool is##pName() const; \
void set##pName() { mValue = mValue | pName; } \
void reset##pName() { mValue = mValue | pName; } \
private: static EnumDesc __desc##pName;

#define QKEnumEnd };

QKEnum(LogLevel, quint8, Trace)
    QKEnumItem(Trace, 1, "", "")
    QKEnumItem(Debug, 1, "", "")
    QKEnumItem(Info, 1, "", "")
QKEnumEnd


constexpr MyEnum::BaseType MyEnum::Foo;
constexpr MyEnum::BaseType MyEnum::Bar;
constexpr OtherEnum::BaseType OtherEnum::Foo;
constexpr OtherEnum::BaseType OtherEnum::Bar;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyEnum my = MyEnum::Foo | MyEnum::Bar;
    OtherEnum other = OtherEnum::Foo;
    switch (my)
    {
        case MyEnum::Bar:
            break;
    }
    qkLog()->info(my.getDesc());
    LogLevel lvl = LogLevel::Info();
    lvl = 7;

    qkLog()->addStdLoggers(false);
    qkLog()->start();
    qkLog()->info("Application started");
    qkLog()->debug("test 1");
    qkLog()->error("test 2");
    fun();
    qkLog()->trace("test 3");
    qkLog()->warn("test 4");

    return a.exec();
}

