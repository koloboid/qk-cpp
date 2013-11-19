# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = lib
DESTDIR = dist/Release/GNU-Linux-x86
TARGET = qk.mod
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += staticlib release 
PKGCONFIG +=
QT = core network
SOURCES += ctimemodule.cpp modmanager.cpp
HEADERS += ctimemodule.hpp modmanager.hpp
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Release/GNU-Linux-x86
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
