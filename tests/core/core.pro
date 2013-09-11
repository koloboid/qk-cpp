#-------------------------------------------------
#
# Project created by QtCreator 2013-08-19T07:08:22
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = qkcore.test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_LFLAGS += -Wl,--no-undefined

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../core/release/ -lqk.core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../core/debug/ -lqk.core
else:unix: LIBS += -L$$OUT_PWD/../../core/ -lqk.core

INCLUDEPATH += $$PWD/../../core
DEPENDPATH += $$PWD/../../core
