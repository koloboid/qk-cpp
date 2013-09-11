#-------------------------------------------------
#
# Project created by QtCreator 2013-08-19T07:02:57
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = dbmysql
TEMPLATE = lib

DEFINES += DBMYSQL_LIBRARY

SOURCES += qkdbmysqldriver.cpp

HEADERS += qkdbmysqldriver.hpp\
        dbmysql_global.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
