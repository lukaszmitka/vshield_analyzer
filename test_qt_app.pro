#-------------------------------------------------
#
# Project created by QtCreator 2016-06-22T21:24:04
#
#-------------------------------------------------

QT       += core gui
QT      += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_qt_app
TEMPLATE = app

QTPLUGIN += QSQLITE

SOURCES += main.cpp\
        mainwindow.cpp \
    vshieldreader.cpp \
    shield.cpp \
    facestate.cpp

HEADERS  += mainwindow.h \
    vshieldreader.h \
    shield.h \
    facestate.h

FORMS    += mainwindow.ui
