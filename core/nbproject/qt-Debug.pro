# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = lib
DESTDIR = dist/Debug/GNU-Linux-x86
TARGET = core
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += staticlib debug 
PKGCONFIG +=
QT = core network
SOURCES += config.cpp consoledevice.cpp enum.cpp enumitem.cpp error.cpp lock.cpp log.cpp log.glibc.cpp logger.cpp loggertextio.cpp logitem.cpp loglevel.cpp timespan.cpp
HEADERS += blob.hpp blockqueue.hpp config.hpp consoledevice.hpp core.export.hpp enum.hpp enumitem.hpp error.hpp lock.hpp log.hpp logger.hpp loggertextio.hpp logitem.hpp loglevel.hpp timespan.hpp valref.hpp
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += 
LIBS += 
equals(QT_MAJOR_VERSION, 4) {
QMAKE_CXXFLAGS += -std=c++11
}
equals(QT_MAJOR_VERSION, 5) {
CONFIG += c++11
}
