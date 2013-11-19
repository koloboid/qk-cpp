# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = lib
DESTDIR = dist/Debug/GNU-Linux-x86
TARGET = qk.db
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += staticlib debug 
PKGCONFIG +=
QT = core sql
SOURCES += condition.cpp db.cpp driver.cpp drivermysql.cpp errordb.cpp field.cpp fieldflag.cpp model.cpp query.cpp rowaction.cpp rowstate.cpp table.cpp value.cpp
HEADERS += condition.hpp db.export.hpp db.hpp dbmacro.hpp driver.hpp drivermysql.hpp errordb.hpp field.hpp fieldflag.hpp model.hpp query.hpp rowaction.hpp rowstate.hpp table.hpp value.hpp
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/GNU-Linux-x86
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += /home/k0l0b0k/project/active/ggexplorer/gge2/trunk 
LIBS += 
equals(QT_MAJOR_VERSION, 4) {
QMAKE_CXXFLAGS += -std=c++11
}
equals(QT_MAJOR_VERSION, 5) {
CONFIG += c++11
}
