QT += core
QT -= gui
QT += network
CONFIG += c++11

TARGET = WSSMaster
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    masterobject.cpp \
    crypt.cpp \
    serveur.cpp \
    sockettcp.cpp \
    socketudp.cpp

HEADERS += \
    masterobject.h \
    crypt.h \
    serveur.h \
    sockettcp.h \
    socketudp.h
