#-------------------------------------------------
#
# Project created by QtCreator 2014-03-10T15:48:58
#
#-------------------------------------------------

QT       += widgets qml quick svg webkit webkitwidgets xml

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = qk.ui.server
TEMPLATE = lib
CONFIG += staticlib c++11
INCLUDEPATH += ../../../
QMAKE_CXXFLAGS += -Wall

SOURCES += \
    widget.cpp

HEADERS += \
    widget.hpp

OTHER_FILES += \
    cmd.json
