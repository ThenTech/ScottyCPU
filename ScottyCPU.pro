TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    SynchronousComponent.h \
    Gate.h \
    ANDGate.h \
    ORGate.h \
    NORGate.h \
    NANDGate.h \
    NOTGate.h \
    XORGate.h \
    UnitTest.h \
    MultiGate.h \
    Exceptions.h \
    MultiANDGate.h
