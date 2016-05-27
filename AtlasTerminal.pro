#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T12:51:49
#
#-------------------------------------------------

QT       += core gui
QT      += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AtlasTerminal
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    atlasdialog.cpp \
    qatlas.cpp \
    qatlasusb.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    atlasdialog.h \
    qatlas.h \
    qatlasusb.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    atlasdialog.ui

RESOURCES += \
    atlasterminal.qrc

CONFIG += c++11
CONFIG += warn_on

QMAKE_CXXFLAGS += -Wextra

INCLUDEPATH += ..

include ( C:\qwt-6.1.2\features\qwt.prf )

//INCLUDEPATH += D:\PROJECTS_LTD\Qt\QtPlugins\led-designer-plugin
//win32: LIBS += D:\PROJECTS_LTD\Qt\QtPlugins\led-designer-plugin\build-led-designer-plugin\release\led-designer-plugin.dll
