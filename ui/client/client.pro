#-------------------------------------------------
#
# Project created by QtCreator 2014-03-10T15:48:58
#
#-------------------------------------------------

QT       += widgets qml quick svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = qk.ui.client
TEMPLATE = lib
CONFIG += staticlib c++11
INCLUDEPATH += ../../../
QMAKE_CXXFLAGS += -Wall

SOURCES += \
    qkuiapplication.cpp \
    viewhost.cpp \
    host.cpp \
    messagebox.cpp

HEADERS += \
    qkuiapplication.hpp \
    viewhost.hpp \
    host.hpp \
    messagebox.hpp

FORMS += \
    desktop/messagebox.ui

OTHER_FILES +=

RESOURCES += \
    res.qrc
