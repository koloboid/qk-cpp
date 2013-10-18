#pragma once

#include <qkconfig.hpp>

class TestConf : public QkConfig
{
    Q_OBJECT
public:
    explicit TestConf(QObject *parent = 0);

public:
    QK_CONFIGPROP(QString, dbconn, "driver=mysql; user=root; dbname=test; host=localhost; port=3306", "-db",
                  "Database connection string")
};
