QT += testlib
QT += network
QT -= gui
TARGET = Tests
DESTDIR = "output/debug"
MOC_DIR = ".tmp/debug"
OBJECTS_DIR = ".tmp/debug"

#LIBS += -L../../output/debug \
#    -lNetworkManager

INCLUDEPATH += . \
    ../ \
    ../../ \
    ../../../ \
    ../../../../ \
    ../../../../../ \
    ../../../../../../ \
    ../../../../../../../
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    Tests.cpp
HEADERS += Tests.h