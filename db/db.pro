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
    db.cpp \
    field.cpp \
    row.cpp \
    driver.cpp \
    errordb.cpp \
    drivermysql.cpp \
    condition.cpp \
    query.cpp \
    table.cpp

HEADERS += \
    db.hpp \
    field.hpp \
    row.hpp \
    driver.hpp \
    errordb.hpp \
    drivermysql.hpp \
    condition.hpp \
    query.hpp \
    fieldflag.hpp \
    table.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
