#-------------------------------------------------
#
# Project created by QtCreator 2015-12-21T12:18:23
#
#-------------------------------------------------

QT          += core gui sql

CONFIG      += c++11  #required by libmapper/lo
CONFIG      += no_keywords #signals


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-mapper-versioning-test
TEMPLATE = app

unix:macx { #using default *nix mapper/lo configurations

DEFINES     += __MAC_OR_UNIX__ # in case we want some specific settings

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/mapper-0

LIBS        += -L/usr/local/lib -lgit2
LIBS        += -L/usr/local/lib/ -llo
LIBS        += -L/usr/local/lib/ -lmapper-0

}

win32 {
#win32 specific pthread, liblo, and mapper header/lib locations here
}


SOURCES     += main.cpp\
            mainwindow.cpp \
    mapperdbthread.cpp \
    testmapperdevice.cpp \
    linkview.cpp \
    signallist.cpp \
    listtab.cpp \
    tab.cpp \
    connectionprops.cpp \
    mapperUI/customrect.cpp \
    mapperUI/qmapperdbmodel.cpp \
    mapperUI/qmapperdbscene.cpp \
    mapperUI/qmapperscenelayer.cpp \
    mapperData/mapperjsonconfig.cpp \
    mapperData/mapperdata.cpp \
    versionsdialog.cpp

HEADERS     += mainwindow.h \
    mapperdbthread.h \
    testmapperdevice.h \
    linkview.h \
    signallist.h \
    listtab.h \
    tab.h \
    connectionprops.h \
    mapperUI/customrect.h \
    mapperUI/qmapperdbmodel.h \
    mapperUI/qmapperdbscene.h \
    mapperUI/qmapperscenelayer.h \
    mapperData/mapperjsonconfig.h \
    mapperData/mapperdata.h \
    versionsdialog.h

FORMS       += mainwindow.ui \
    linkview.ui \
    signallist.ui \
    connectionprops.ui \
    versionsdialog.ui
