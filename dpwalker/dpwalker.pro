# .pro modified by C6H6Vita <c6h6@outlook.kr>


TEMPLATE = app
TARGET = DPWalker
DEPENDPATH += . .
INCLUDEPATH += .

QT+=widgets

# Input
HEADERS += dependencyjob.h \
           dependsui.h \
           importsexportsjob.h
FORMS += dependsuibase.ui
SOURCES += dependencyjob.cpp \
           dependsui.cpp \
           importsexportsjob.cpp \
           main.cpp

RESOURCES += dpwalker.qrc


#This compiler flag fixes Ubuntu 18.04 - Specific problems:
#Fix MIME type: Shared object(Default) to Executable

QMAKE_LFLAGS += -no-pie
