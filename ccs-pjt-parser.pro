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
    projectreader.cpp \
    export/abstractprojectexport.cpp \
    export/projectexportccs3.cpp \
    buildstep.cpp \
    buildsteplist.cpp \
    fileoptions.cpp

HEADERS += \
    projectparser.h \
    projectsettings.h \
    utils.h \
    configsettings.h \
    projectreader.h \
    export/abstractprojectexport.h \
    export/projectexportccs3.h \
    buildstep.h \
    buildsteplist.h \
    fileoptions.h
