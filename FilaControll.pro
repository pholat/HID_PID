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
    ControlTypes/SetupSwitch.cpp \
    ControlTypes/setup.cpp \
    ControlTypes/setupbistate.cpp \
    ControlTypes/setuptristate.cpp \
    ControlTypes/setuptempcheck.cpp \
    ControlTypes/setupexttempcurv.cpp \
    ControlTypes/setupsolidering.cpp \
    ControlTypes/setuppidcontroll.cpp \
    QtUsb/baseinfo.cpp \
    QtUsb/usb-container.cpp \
    QtUsb/usbdev.cpp \

HEADERS  += mainwindow.h \
    tempTable.h \
    ControlTypes/SetupSwitch.h \
    ControlTypes/setup.h \
    ControlTypes/setupbistate.h \
    ControlTypes/setuptristate.h \
    ControlTypes/setuptempcheck.h \
    ControlTypes/setupexttempcurv.h \
    ControlTypes/setupsolidering.h \
    ControlTypes/setuppidcontroll.h \
    QtUsb/baseinfo.h \
    QtUsb/usb-container.h \
    QtUsb/usbdev.h

FORMS    += mainwindow.ui

LIBS += -lqwt-qt5 -lusb-1.0

QMAKE_LFLAGS += -Wl,-O2 -Wl,-rpath,./lib
