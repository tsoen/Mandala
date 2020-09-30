#-------------------------------------------------
#
# Project created by QtCreator 2019-03-12T14:29:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ensi-Mandala
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    paintwidget.cpp \
    dimensionspinbox.cpp

HEADERS  += mainwindow.h \
    paintwidget.h \
    dimensionspinbox.h

FORMS    += mainwindow.ui

TRANSLATIONS += translations/mandala_fr.ts\

QMAKE_CXXFLAGS += -std=c++11

CONFIG += c++11

RESOURCES += \
    translations/translations.qrc

DISTFILES += \
    README.md
