#-------------------------------------------------
#
# Project created by QtCreator 2013-11-19T19:02:05
#
#-------------------------------------------------

QT       += network
QT       -= gui

TARGET = qk.rpc
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

HEADERS += \
    server.hpp \
    context.hpp \
    session.hpp \
    handler.hpp \
    sessionstorage.hpp \
    sessionstoragememory.hpp \
    transport.hpp

SOURCES += \
    server.cpp \
    context.cpp \
    session.cpp \
    handler.cpp \
    sessionstorage.cpp \
    sessionstoragememory.cpp \
    transport.cpp
