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
        mainwindow.cpp \
    setup.cpp \
    setupbistate.cpp \
    setuptristate.cpp \
    setuptempcheck.cpp \
    setupexttempcurv.cpp \
    setupsolidering.cpp \
    setuppidcontroll.cpp \
    usbdevcon.cpp

HEADERS  += mainwindow.h \
    tempTable.h \
    setup.h \
    setupbistate.h \
    setuptristate.h \
    setuptempcheck.h \
    setupexttempcurv.h \
    setupsolidering.h \
    setuppidcontroll.h \
    usbdevcon.h

FORMS    += mainwindow.ui

LIBS += -lqwt-qt5 -lusb-1.0

QMAKE_LFLAGS += -Wl,-O2 -Wl,-rpath,./lib
