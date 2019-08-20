#-------------------------------------------------
#
# Project created by QtCreator 2017-12-12T09:11:42
#
#-------------------------------------------------

QT       += core
QT += sql

QT       -= gui

TARGET = DBClient
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    person.cpp \
    service.cpp \
    team.cpp \
    stadium.cpp \
    match.cpp \
    workingpeople.cpp

HEADERS += \
    person.h \
    service.h \
    team.h \
    stadium.h \
    match.h \
    workingpeople.h
