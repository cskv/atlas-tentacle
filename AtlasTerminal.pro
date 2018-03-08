#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T12:51:49
#
#-------------------------------------------------

QT       += core gui
QT      += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = AtlasTerminal
TEMPLATE = app

# Tell the qcustomplot header that it will be used as library:
# DEFINES += QCUSTOMPLOT_USE_LIBRARY

# Link with debug version of qcustomplot if compiling in debug mode, else with release library:
#CONFIG(debug, release|debug) {
#  win32:QCPLIB = qcustomplotd1.dll
#  else: QCPLIB = qcustomplotd
#} else {
#  win32:QCPLIB = qcustomplot1.dll
#  else: QCPLIB = qcustomplot
#}

#win32: LIBS += "../$$QCPLIB"
#unix:  LIBS += -L"../" -l$$QCPLIB
#LIBS += -L"$$_PRO_FILE_PWD_" -l$$QCPLIB

#win32: INCLUDEPATH += "D:\Projects_LTD\Qt\qcustomplot"
#unix:  INCLUDEPATH += "/home/pvk/Projects/Qt/qcustomplot"

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/atlasdialog.cpp \
    src/qatlas.cpp \
    src/qatlasusb.cpp \
    src/ledindicator.cpp \
    src/about.cpp \
    src/ezoframe.cpp \
    src/plotframe.cpp \
    src/serialdialog.cpp \
    src/qtentacle.cpp \
    thirdparty/qcustomplot.cpp

HEADERS += \
    src/mainwindow.h \
    src/atlasdialog.h \
    src/qatlas.h \
    src/qatlasusb.h \
    src/ledindicator.h \
    src/about.h \
    src/ezoframe.h \
    src/plotframe.h \
    src/serialdialog.h \
    src/qtentacle.h \
    thirdparty/qcustomplot.h

FORMS += \
    src/mainwindow.ui \
    src/atlasdialog.ui \
    src/about.ui \
    src/ezoframe.ui \
    src/plotframe.ui \
    src/serialdialog.ui

RESOURCES += \
    atlasterminal.qrc

CONFIG += c++11
CONFIG += warn_on

#QMAKE_CXXFLAGS += -Wextra

INCLUDEPATH += ./thirdparty

#qwt is not necessary any more
#win32: include ( C:\qwt-6.1.2\features\qwt.prf )
#unix: include ( /usr/lib/qt/mkspecs/features/qwt.prf )
