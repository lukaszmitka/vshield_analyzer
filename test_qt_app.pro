#-------------------------------------------------
#
# Project created by QtCreator 2016-06-22T21:24:04
#
#-------------------------------------------------

QT       += core gui
QT      += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = test_qt_app
TEMPLATE = app

QTPLUGIN += QSQLITE

SOURCES += main.cpp\
        mainwindow.cpp \
    vshieldreader.cpp \
    shield.cpp \
    facestate.cpp \
    qcustomplot.cpp \
    pressurelimitdialog.cpp \
    staytimedialog.cpp

HEADERS  += mainwindow.h \
    vshieldreader.h \
    shield.h \
    facestate.h \
    qcustomplot.h \
    pressurelimitdialog.h \
    staytimedialog.h

FORMS    += mainwindow.ui
