TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    SynchronousComponent.h \
    Gate.h \
    UnitTest.h \
    ANDGate.h \
    ORGate.h \
    NORGate.h \
    NANDGate.h \
    NOTGate.h \
    XORGate.h
