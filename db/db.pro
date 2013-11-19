#-------------------------------------------------
#
# Project created by QtCreator 2013-11-19T18:58:59
#
#-------------------------------------------------

QT       += sql
QT       -= gui

TARGET = qk.db
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += ../../
QMAKE_CXXFLAGS += -std=c++0x -Wall

SOURCES += \
    value.cpp \
    table.cpp \
    rowstate.cpp \
    rowaction.cpp \
    query.cpp \
    model.cpp \
    fieldflag.cpp \
    field.cpp \
    errordb.cpp \
    drivermysql.cpp \
    driver.cpp \
    db.cpp \
    condition.cpp

HEADERS += \
    value.hpp \
    table.hpp \
    rowstate.hpp \
    rowaction.hpp \
    query.hpp \
    model.hpp \
    fieldflag.hpp \
    field.hpp \
    errordb.hpp \
    drivermysql.hpp \
    driver.hpp \
    dbmacro.hpp \
    db.hpp \
    db.export.hpp \
    condition.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
