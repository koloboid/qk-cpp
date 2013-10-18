#-------------------------------------------------
#
# Project created by QtCreator 2013-08-23T04:12:14
#
#-------------------------------------------------

QT       += testlib sql
QT       -= gui

TARGET = tst_qkdbtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_LFLAGS += -Wl,--no-undefined

SOURCES += tst_qkdbtest.cpp \
    main.cpp \
    tuser.cpp \
    tusergroup.cpp \
    tentity.cpp \
    tbase.cpp \
    testconf.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../core/release/ -lqk.core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../core/debug/ -lqk.core
else:unix: LIBS += -L$$OUT_PWD/../../core/ -lqk.core

INCLUDEPATH += $$PWD/../../core
DEPENDPATH += $$PWD/../../core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../db/release/ -lqk.db
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../db/debug/ -lqk.db
else:unix: LIBS += -L$$OUT_PWD/../../db/ -lqk.db

INCLUDEPATH += $$PWD/../../db
DEPENDPATH += $$PWD/../../db

HEADERS += \
    tuser.hpp \
    tusergroup.hpp \
    tentity.hpp \
    tbase.hpp \
    testconf.hpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../dbmysql/release/ -ldbmysql
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../dbmysql/debug/ -ldbmysql
else:unix: LIBS += -L$$OUT_PWD/../../dbmysql/ -ldbmysql

INCLUDEPATH += $$PWD/../../dbmysql
DEPENDPATH += $$PWD/../../dbmysql
