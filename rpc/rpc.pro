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

SOURCES += \
    rpcserver.cpp \
    irpchandler.cpp \
    httpcontext.cpp

HEADERS += \
    rpcserver.hpp \
    irpchandler.hpp \
    httpcontext.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
