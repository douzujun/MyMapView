#-------------------------------------------------
#
# Project created by QtCreator 2016-11-25T19:37:50
#
#-------------------------------------------------

QT       += core gui
RC_ICONS += myapp.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyMapView
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    mapwidget.cpp

HEADERS  += mainwindow.h \
    mapwidget.h
