#-------------------------------------------------
#
# Project created by QtCreator 2016-06-22T21:24:04
#
#-------------------------------------------------

#INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtWidgets

QMAKE_CXXFLAGS += -std=c++11


QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport   sql

TARGET = analiza_vshield
TEMPLATE = app

QTPLUGIN += QSQLITE

SOURCES += main.cpp\
        mainwindow.cpp \
    vshieldreader.cpp \
    shield.cpp \
    facestate.cpp \
    qcustomplot.cpp \
    pressurelimitdialog.cpp \
    staytimedialog.cpp \
    pressuregainindexdialog.cpp \
    indexedqcheckbox.cpp \
    vshieldanalyzerworker.cpp \
    vshieldprogressdialog.cpp \
    exportcsvdialog.cpp

HEADERS  += mainwindow.h \
    vshieldreader.h \
    shield.h \
    facestate.h \
    qcustomplot.h \
    pressurelimitdialog.h \
    staytimedialog.h \
    pressuregainindexdialog.h \
    indexedqcheckbox.h \
    vshieldanalyzerworker.h \
    vshieldprogressdialog.h \
    exportcsvdialog.h

FORMS    += mainwindow.ui
