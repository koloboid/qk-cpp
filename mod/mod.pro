#-------------------------------------------------
#
# Project created by QtCreator 2013-11-19T18:58:59
#
#-------------------------------------------------

QT       -= gui

TARGET = qk.mod
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += ../../
QMAKE_CXXFLAGS += -std=c++0x -Wall

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
    ctimemodule.cpp

HEADERS += \
    modmanager.hpp \
    ctimemodule.hpp
