#-------------------------------------------------
#
# Project created by QtCreator 2012-01-17T23:22:31
#
#-------------------------------------------------

QT       += core gui

TARGET = QTomato
TEMPLATE = app


SOURCES += src/main.cpp \
    src/qtomatotray.cpp \
    src/qtomatotimer.cpp \
    src/qtomatoconfigdialog.cpp \
    src/qtomatoconfig.cpp

HEADERS  += \
    src/qtomatotray.h \
    src/qtomatotimer.h \
    src/qtomatoconfigdialog.h \
    src/qtomatoconfig.h

RESOURCES += \
    QTomato.qrc

FORMS += \
    src/qtomatoconfigdialog.ui

TRANSLATIONS += \
    l10n/qtomato_nl.ts
