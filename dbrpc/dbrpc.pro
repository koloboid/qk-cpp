#-------------------------------------------------
#
# Project created by QtCreator 2013-11-19T19:04:17
#
#-------------------------------------------------

QT       += network sql
QT       -= gui

TARGET = qk.dbrpc
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += ../../
QMAKE_CXXFLAGS += -std=c++0x -Wall

SOURCES += \
    tablehandler.cpp

HEADERS += \
    tablehandler.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
