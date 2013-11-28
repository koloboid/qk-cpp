#-------------------------------------------------
#
# Project created by QtCreator 2013-11-23T08:16:11
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = qk.rpchttp
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += ../../
QMAKE_CXXFLAGS += -std=c++0x -Wall
LIBS += -ltufao1

SOURCES += \
    httptransport.cpp \
    httpcontext.cpp \
    httppostformatter.cpp

HEADERS += \
    httptransport.hpp \
    httpcontext.hpp \
    httppostformatter.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
