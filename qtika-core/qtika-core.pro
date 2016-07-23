#-------------------------------------------------
#
# Project created by QtCreator 2016-07-22T20:45:35
#
#-------------------------------------------------

QT       -= gui

TARGET = qtika-core
TEMPLATE = lib
CONFIG += warn_on

DEFINES += QTIKACORE_LIBRARY

SOURCES += \
    mime/mediatype.cpp

HEADERS += \
    qtika-core-global.h \
    mime/mediatype.h \
    stable.h

#PRECOMPILED_HEADER = qtika-core-global.h

win32: QMAKE_CXXFLAGS_WARN_ON = /W4

unix {
    target.path = /usr/lib
    INSTALLS += target
}
