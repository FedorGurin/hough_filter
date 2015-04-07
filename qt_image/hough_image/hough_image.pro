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
    ../../pureC/c_hough_asym.cpp \
    ../../pureC/c_hough_asym_sec.cpp

HEADERS  += mainwindow.h \
    ../../pureC/c_hough_asym.h \
    ../../pureC/c_hough_asym_sec.h

FORMS    += mainwindow.ui
