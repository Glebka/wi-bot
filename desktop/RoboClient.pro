#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T11:14:29
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RoboClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    roboclient.cpp \
    CommandBuilder.cpp

HEADERS  += mainwindow.h \
    roboclient.h \
    CommandBuilder.hpp

FORMS    += mainwindow.ui
