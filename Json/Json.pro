#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T14:55:07
#
#-------------------------------------------------

QT       += scripttools

QT       -= gui

TARGET = Json
TEMPLATE = lib

DESTDIR         = ../build

DEFINES += JSON_LIBRARY

SOURCES += json.cpp

HEADERS += json.h
HEADERS += json_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
