#-------------------------------------------------
#
# Project created by QtCreator 2016-08-16T18:29:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PathTest
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    screen.cpp \
    myrect.cpp \
    controlwidget.cpp

HEADERS  += widget.h \
    screen.h \
    myrect.h \
    controlwidget.h

FORMS    += widget.ui \
    controlwidget.ui

RESOURCES += \
    res.qrc
