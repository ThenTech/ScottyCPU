TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

RC_FILE = ScottyResources.rc

SOURCES += main.cpp

HEADERS += \
    Exceptions.hpp \
    SynchrotronComponent.hpp \
    UnitTest.hpp \
    utils.hpp \
    SynchrotronComponentFixedInput.hpp \
    SynchrotronComponentEnable.hpp \
    Clock.hpp \
    SignedBitset.hpp \
    ScottyCPU.hpp \
    CPUComponents/ALUnit.hpp \
    CPUComponents/ANDGate.hpp \
    CPUComponents/Clock.hpp \
    CPUComponents/ControlUnit.hpp \
    CPUComponents/CPUComponentFactory.hpp \
    CPUComponents/Memory.hpp \
    CPUComponents/MemoryCell.hpp \
    CPUComponents/NANDGate.hpp \
    CPUComponents/NORGate.hpp \
    CPUComponents/NOTGate.hpp \
    CPUComponents/ORGate.hpp \
    CPUComponents/SHIFTLeft.hpp \
    CPUComponents/SHIFTRight.hpp \
    CPUComponents/XORGate.hpp \
    CPUInstructions/ANDInstruction.hpp \
    CPUInstructions/ORInstruction.hpp \
    CPUInstructions/NANDInstruction.hpp \
    CPUInstructions/NORInstruction.hpp \
    CPUInstructions/NOTInstruction.hpp \
    CPUInstructions/XORInstruction.hpp \
    CPUInstructions/SHLInstruction.hpp \
    CPUInstructions/SHRInstruction.hpp \
    CPUComponents/ADD.hpp \
    CPUComponents/SUBTRACT.hpp \
    CPUInstructions/ADDInstruction.hpp \
    CPUInstructions/SUBInstruction.hpp \
    CPUComponents/MULTIPLY.hpp \
    CPUInstructions/MULInstruction.hpp \
    FloatingBitset.hpp \
    CPUComponents/DIVIDE.hpp \
    CPUInstructions/DIVInstruction.hpp \
    CPUComponents/MODULO.hpp \
    CPUInstructions/MODInstruction.hpp \
    CPUComponents/COMPERATOR.hpp \
    CPUInstructions/CMPInstruction.hpp \
    CPUInstructions/Instruction.hpp \
    Assembler.hpp \
    CPUFactory/SCAMParser.hpp \
    CPUFactory/SCAMAssembler.hpp

DISTFILES += \
    Programs/example.scam \
    Programs/example_corrected.scam \
    Programs/example_corrected.schex \
    Programs/square.scam \
    ScottyResources.rc
