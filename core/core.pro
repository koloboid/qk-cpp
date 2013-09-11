#-------------------------------------------------
#
# Project created by QtCreator 2013-08-19T07:01:31
#
#-------------------------------------------------

QT       -= gui

TARGET = qk.core
TEMPLATE = lib

DEFINES += QKCORE_LIBRARY QK_EXCEPTION

SOURCES += \
    qklog.cpp \
    qkenum.cpp \
    qklogger.cpp \
    qkloggertextio.cpp \
    qkconsoledevice.cpp \
    qkconfig.cpp \
    qkerror.cpp \
    qktimespan.cpp \
    qklock.cpp \
    qklogitem.cpp \
    qkloglevel.cpp \
    qklog.glibc.cpp \
    qkenumitem.cpp

HEADERS += \
    qklog.hpp \
    qkenum.hpp \
    qklogger.hpp \
    qkloggertextio.hpp \
    qkconsoledevice.hpp \
    qkblob.hpp \
    qkconfig.hpp \
    qkthrowable.hpp \
    qkerror.hpp \
    qktimespan.hpp \
    qklock.hpp \
    qkcore.export.hpp \
    qkvalref.hpp \
    qklogitem.hpp \
    qkloglevel.hpp \
    qkblockqueue.hpp \
    qkenumitem.hpp

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_LFLAGS += -Wl,--no-undefined

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
