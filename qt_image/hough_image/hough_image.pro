#-------------------------------------------------
#
# Project created by QtCreator 2015-03-27T00:13:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hough_image
TEMPLATE = app
INCLUDEPATH += ../../pureC/


SOURCES += main.cpp\
        mainwindow.cpp \
    ../../pureC/hough_c.cpp

HEADERS  += mainwindow.h \
    ../../pureC/hough_c.h

FORMS    += mainwindow.ui
