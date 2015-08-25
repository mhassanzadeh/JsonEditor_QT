#-------------------------------------------------
#
# Project created by QtCreator 2015-05-26T09:21:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConfigView
TEMPLATE = app

LIBS += -L../build -lJson

SOURCES += main.cpp \
    syntaxhighlighter.cpp
SOURCES += mainwindow.cpp

HEADERS  += mainwindow.h \
    syntaxhighlighter.h

FORMS    += mainwindow.ui
