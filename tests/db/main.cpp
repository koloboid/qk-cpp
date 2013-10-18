#include <qkdb.hpp>
#include <QDebug>
#include <qkdbmysqldriver.hpp>

#include "tuser.hpp"
#include "tusergroup.hpp"
#include "testconf.hpp"
#include <QThreadStorage>
#include <qkerror.hpp>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    TestConf conf;
    conf.init(argc, argv);
    if (conf.handleHelp()) return 0;

    QkDb db;
    db.setupConnection(conf.dbconn());
    User = db.registerTable<TUser>();
    db.migrate(); // sync

    QkDbRow row = User->newRow();
    row.save(); // sync? save, throw or false
    row.save([](const QkError& pErr, bool pRv) {

    }); // async, save, return error as param

    return app.exec();
}
