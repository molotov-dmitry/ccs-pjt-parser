TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11

SOURCES += main.cpp \
    projectparser.cpp \
    projectsettings.cpp \
    utils.cpp \
    configsettings.cpp \
    projectreader.cpp

HEADERS += \
    projectparser.h \
    projectsettings.h \
    utils.h \
    configsettings.h \
    projectreader.h
