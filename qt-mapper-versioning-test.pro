#-------------------------------------------------
#
# Project created by QtCreator 2015-12-21T12:18:23
#
#-------------------------------------------------

QT          += core gui

CONFIG      += c++11  #required by libmapper/lo


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-mapper-versioning-test
TEMPLATE = app

unix:macx { #using default *nix mapper/lo configurations

DEFINES     += __MAC_OR_UNIX__ # in case we want some specific settings

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/mapper-0

LIBS        += -L/usr/local/lib/ -llo
LIBS        += -L/usr/local/lib/ -lmapper-0

}

win32 {
#win32 specific pthread, liblo, and mapper header/lib locations here
}


SOURCES     += main.cpp\
            mainwindow.cpp

HEADERS     += mainwindow.h

FORMS       += mainwindow.ui
