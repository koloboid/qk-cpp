#-------------------------------------------------
#
# Project created by QtCreator 2013-11-19T18:58:59
#
#-------------------------------------------------

QT       -= gui

TARGET = qk.mod
TEMPLATE = lib
CONFIG += staticlib c++11
INCLUDEPATH += ../../
QMAKE_CXXFLAGS += -Wall

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

SOURCES += \
    modmanager.cpp \
    compiletimemodule.cpp

HEADERS += \
    modmanager.hpp \
    compiletimemodule.hpp
