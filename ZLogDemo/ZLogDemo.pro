#-------------------------------------------------
#
# Project created by QtCreator 2018-06-07T23:14:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZLogDemo
TEMPLATE = app

include(zLog/z_log.pri)


SOURCES += main.cpp\
        Dialog.cpp

HEADERS  += Dialog.h

FORMS    += Dialog.ui
