#include <QCoreApplication>
#include <qklog.hpp>

void fun()
{
    QkLogScope("");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

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

