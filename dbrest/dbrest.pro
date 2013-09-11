#-------------------------------------------------
#
# Project created by QtCreator 2013-08-19T07:04:59
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = dbrest
TEMPLATE = lib

DEFINES += DBREST_LIBRARY

SOURCES += qkdbrestdriver.cpp

HEADERS += qkdbrestdriver.hpp\
        dbrest_global.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
