#-------------------------------------------------
#
# Project created by QtCreator 2014-07-17T08:59:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FilaControll
TEMPLATE = app
CONFIG += c++11


SOURCES += \
    UI/main.cpp\
    UI/mainwindow.cpp \
    Controll/SetupSwitch.cpp \
    Controll/setup.cpp \
    Controll/setupbistate.cpp \
    Controll/setuptristate.cpp \
    Controll/setuptempcheck.cpp \
    Controll/setupexttempcurv.cpp \
    Controll/setupsolidering.cpp \
    Controll/setuppidcontroll.cpp \
    Extern/pid/pid.cpp \
    Controll/ComPoll.cpp \

HEADERS  += \
    UI/mainwindow.h \
    Controll/tempTable.h \
    Controll/SetupSwitch.h \
    Controll/setup.h \
    Controll/setupbistate.h \
    Controll/setuptristate.h \
    Controll/setuptempcheck.h \
    Controll/setupexttempcurv.h \
    Controll/setupsolidering.h \
    Controll/setuppidcontroll.h \
    Extern/pid/pid.h \
    Controll/ComPoll.h \

FORMS    += UI/mainwindow.ui

LIBS += -lqwt-qt5 -lusb-1.0

QMAKE_LFLAGS += -Wl,-O2 -Wl,-rpath,./lib
