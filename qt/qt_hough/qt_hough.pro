#-------------------------------------------------
#
# Project created by QtCreator 2015-03-22T17:09:52
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = qt_hough
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH += ../../pureC/

SOURCES += main.cpp \
    ../../pureC/hough_c.cpp

HEADERS += \
    ../../pureC/hough_c.h
