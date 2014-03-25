#-------------------------------------------------
#
# Project created by QtCreator 2013-11-19T18:48:34
#
#-------------------------------------------------

QT       -= gui
QT       += network xml

TARGET = qk.core
TEMPLATE = lib
CONFIG += staticlib c++11
INCLUDEPATH += ../../
QMAKE_CXXFLAGS += -Wall

SOURCES += \
    timespan.cpp \
    loglevel.cpp \
    logitem.cpp \
    loggertextio.cpp \
    logger.cpp \
    log.cpp \
    lock.cpp \
    error.cpp \
    consoledevice.cpp \
    config.cpp \
    config.unix.cpp \
    formatterjson.cpp \
    formatterxml.cpp \
    qkapplication.cpp \
    formatter.cpp \
    serializable.cpp

HEADERS += \
    valref.hpp \
    timespan.hpp \
    loglevel.hpp \
    logitem.hpp \
    loggertextio.hpp \
    logger.hpp \
    log.hpp \
    lock.hpp \
    error.hpp \
    enum.hpp \
    core.export.hpp \
    consoledevice.hpp \
    config.hpp \
    blockqueue.hpp \
    blob.hpp \
    qkapplication.hpp \
    formatterjson.hpp \
    formatterxml.hpp \
    formatter.hpp \
    serializable.hpp

linux-g++|linux-g++-64|linux-g++-32 {
    SOURCES += \
        log.glibc.cpp \
} else {
    SOURCES += \
        log.dummy.cpp \
}

