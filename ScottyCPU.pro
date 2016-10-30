TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    ANDGate.hpp \
    Exceptions.hpp \
    SynchrotronComponent.hpp \
    UnitTest.hpp \
    utils.hpp \
    NANDGate.hpp \
    ORGate.hpp \
    NORGate.hpp \
    XORGate.hpp \
    NOTGate.hpp \
    MemoryCell.hpp \
    SynchrotronComponentFixedInput.hpp \
    SynchrotronComponentEnable.hpp \
    SHIFTRight.hpp \
    SHIFTLeft.hpp \
    Memory.hpp \
    Clock.hpp \
    SignedBitset.hpp \
    CPUComponentFactory.hpp \
    Instructions/BinaryInstruction.hpp \
    Instructions/Instruction.hpp \
    Instructions/UnaryInstruction.hpp \
    ScottyCPU.hpp \
    ControlUnit.hpp \
    ALUnit.hpp
