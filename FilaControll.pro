#-------------------------------------------------
#
# Project created by QtCreator 2014-07-17T08:59:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FilaControll
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    tempTable.h

FORMS    += mainwindow.ui

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../../usr/lib/i386-linux-gnu/ -lusb-1.0

INCLUDEPATH += $$PWD/../../../../../usr/include/libusb-1.0
DEPENDPATH += $$PWD/../../../../../usr/include/libusb-1.0

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/i386-linux-gnu/libusb.a

QMAKE_LFLAGS += -Wl,-O1 -Wl,-rpath,./lib

CONFIG += qwt
