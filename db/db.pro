#-------------------------------------------------
#
# Project created by QtCreator 2013-08-19T07:02:22
#
#-------------------------------------------------

QT       -= gui
QT       += sql

TARGET = qk.db
TEMPLATE = lib

DEFINES += QKDB_LIBRARY

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_LFLAGS += -Wl,--no-undefined

SOURCES += qkdb.cpp \
    qkdbtable.cpp \
    qkdbtableschema.cpp \
    qkdbrow.cpp \
    qkdbfield.cpp \
    qkdbindex.cpp \
    qkdbdriver.cpp \
    qkdbdrv.cpp \
    qkdbresult.cpp \
    qkdbmodel.cpp \
    qkdbfieldflag.cpp \
    qkdbrowstate.cpp \
    qkdbrowaction.cpp \
    qkdbcondition.cpp \
    qkdbquery.cpp

HEADERS += qkdb.hpp\
    qkdbtable.hpp \
    qkdbtableschema.hpp \
    qkdbrow.hpp \
    qkdbfield.hpp \
    qkdbindex.hpp \
    qkdbdriver.hpp \
    qkdbdrv.hpp \
    qkdbresult.hpp \
    qkdbmodel.hpp \
    qkdbfieldflag.hpp \
    qkdbrowstate.hpp \
    qkdbrowaction.hpp \
    qkdbcondition.hpp \
    qkdbquery.hpp \
    qkdb.export.hpp \
    qkdbvalue.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lqk.core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lqk.core
else:unix: LIBS += -L$$OUT_PWD/../core/ -lqk.core

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core
