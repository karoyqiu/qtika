#-------------------------------------------------
#
# Project created by QtCreator 2016-08-13T21:57:26
#
#-------------------------------------------------

QT       -= gui

TARGET = qtika-internal
TEMPLATE = lib
CONFIG += staticlib
CONFIG += warn_on c++11
CONFIG += skip_target_version_ext

VERSION = 1.13

SOURCES += qiodevicetransactionrollback.cpp

HEADERS += qiodevicetransactionrollback.h

win32: QMAKE_CXXFLAGS_WARN_ON = /W4

unix {
    target.path = /usr/lib
    INSTALLS += target
}
