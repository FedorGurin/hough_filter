#-------------------------------------------------
#
# Project created by QtCreator 2015-03-27T00:13:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#QMAKE_LIBS+=-static -lgomp -lpthread
TARGET = hough_image
TEMPLATE = app
INCLUDEPATH += ../../pureC/
LIBS += -openmp
QMAKE_CXXFLAGS += -openmp
QMAKE_LFLAGS += -openmp

SOURCES += main.cpp\
        mainwindow.cpp \    
    ../../pureC/cpu_hough_asym.cpp

HEADERS  += mainwindow.h \    
    ../../pureC/cpu_hough_asym.h

FORMS    += mainwindow.ui
