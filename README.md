# [ScottyCPU](http://scottycpufor.me/)
[![Build status](https://ci.appveyor.com/api/projects/status/0exw7bal3h8n53cp?svg=true)](https://ci.appveyor.com/project/Wosser1sProductions/scottycpu) [![Issue Count](https://codeclimate.com/github/Wosser1sProductions/ScottyCPU/badges/issue_count.svg)](https://codeclimate.com/github/Wosser1sProductions/ScottyCPU)  [![DrMemory errors](https://img.shields.io/badge/DrMemory-0_errors-brightgreen.svg)](http://www.drmemory.org/) [![GitHub issues](https://img.shields.io/github/issues/Wosser1sProductions/ScottyCPU.svg)](https://github.com/Wosser1sProductions/ScottyCPU/issues)  [![progress](https://img.shields.io/badge/total%20progress-87%25-green.svg)](http://doc.scottycpufor.me) [![license](https://img.shields.io/badge/license-GNU%20GPLv3-blue.svg)](https://github.com/Wosser1sProductions/ScottyCPU/blob/master/License.txt)

The base code for a "Scotty Processor" as described by *J. Clark Scott* in his book [But How Do It Know?](http://www.buthowdoitknow.com/).

> But How Do It Know? is the breakthrough book that distills Computer Science down to the simple basic principles that 
all computers have in common, and presents the big picture step by step in plain English so that anyone can understand 
this ingenious, yet basically simple invention called a computer.


For an school project, we will created this CPU model for educational purposes with:
 - Complete functionality regarding a simple processor system e.g. parsing and assembling code and emulating its execution
 - Providing visual information when running assembly programs on the CPU (e.g. memory dumps, step-by-step execution...)
 
By using C++11 and eventually the Qt framework for a GUI.

Documentation has been generated with [Doxygen](http://www.doxygen.org/index.html) and can be found [here](http://doc.scottycpufor.me/) or as an archive in the `release` section.

This batchfile can be used to copy files from working dir to GitHub dir for committing: [gitCPY.bat](https://gist.github.com/Wosser1sProductions/638ff2219b8786a83df863c574a8e58d)

For building, the included [Makefile](https://github.com/Wosser1sProductions/ScottyCPU/blob/master/Makefile) provides common build methods.

###Usage
    Usage: ScottyCPU.exe [-h|-H] [-d|-D] [-c|-C <float>] [-i|-I]
                         [-l|-L|-a|-A <file>] [-o <file>] [-hex <file>]
      -h, -H, --help     Show this help message
      -d, -D, --debug    Execute UnitTests
      -c, -C  <float>    Set the ScottyCPU clock frequency
      -i, -I             Show InstructionSet
      -l, -L  <file>     Load .ScAM file and parse
      -a, -A  <file>     Load .ScAM file and compile to .ScHex
      -o      <file>     Specify output file for assembly
      -hex    <file>     Load .ScHex file into ScottyCPU RAM

###Instruction Set
    /----------------------------------------------------------------------------\
    |------------------------- ScottyCPU InstructionSet -------------------------|
    \----------------------------------------------------------------------------/
    
        |   Name   | OpCode | Operand code | Operand 1 type | Operand 2 type |
        +----------+--------+--------------+----------------+----------------+
        |  NOP     |  0x00  |  0000 0000   |        None    |        None    |
        |  ADD     |  0x01  |  0000 0001   |    Register    |    Register    |
        |  SUB     |  0x02  |  0000 0010   |    Register    |    Register    |
        |  MUL     |  0x03  |  0000 0011   |    Register    |    Register    |
        |  DIV     |  0x04  |  0000 0100   |    Register    |    Register    |
        |  MOD     |  0x05  |  0000 0101   |    Register    |    Register    |
        |  SHL     |  0x06  |  0000 0110   |    Register    |        None    |
        |  SHR     |  0x07  |  0000 0111   |    Register    |        None    |
        |  NOT     |  0x11  |  0001 0001   |    Register    |        None    |
        |  AND     |  0x12  |  0001 0010   |    Register    |    Register    |
        |  NAND    |  0x13  |  0001 0011   |    Register    |    Register    |
        |  OR      |  0x14  |  0001 0100   |    Register    |    Register    |
        |  NOR     |  0x15  |  0001 0101   |    Register    |    Register    |
        |  XOR     |  0x16  |  0001 0110   |    Register    |    Register    |
        |  CMP     |  0x1F  |  0001 1111   |    Register    |    Register    |
        |  MOV     |  0x20  |  0010 0000   |    Register    |    Register    |
        |  MOVRR   |  0x20  |  0010 0000   |    Register    |    Register    |
        |  MOVRA   |  0x22  |  0010 0010   |    Register    |       Value    |
        |  MOVAR   |  0x24  |  0010 0100   |       Value    |    Register    |
        |  MOVAA   |  0x28  |  0010 1000   |       Value    |       Value    |
        |  DATA    |  0x31  |  0011 0001   |    Register    |       Value    |
        |  DATAC   |  0x32  |  0011 0010   |    Register    |       Label    |
        |  JMP     |  0x40  |  0100 0000   |       Label    |        None    |
        |  JE      |  0x41  |  0100 0001   |       Label    |        None    |
        |  JL      |  0x42  |  0100 0010   |       Label    |        None    |
        |  JS      |  0x43  |  0100 0011   |       Label    |        None    |
        |  JZ      |  0x44  |  0100 0100   |       Label    |        None    |
        |  JLE     |  0x50  |  0101 0000   |       Label    |        None    |
        |  JSE     |  0x51  |  0101 0001   |       Label    |        None    |
        |  JCE     |  0x52  |  0101 0010   |       Label    |        None    |
        |  JCL     |  0x53  |  0101 0011   |       Label    |        None    |
        |  JCS     |  0x54  |  0101 0100   |       Label    |        None    |
        |  JCZ     |  0x55  |  0101 0101   |       Label    |        None    |
        |  JLZ     |  0x56  |  0101 0110   |       Label    |        None    |
        |  JSZ     |  0x57  |  0101 0111   |       Label    |        None    |
        |  JEZ     |  0x58  |  0101 1000   |       Label    |        None    |
        |  JCLE    |  0x60  |  0110 0000   |       Label    |        None    |
        |  JCLZ    |  0x61  |  0110 0001   |       Label    |        None    |
        |  JCEZ    |  0x62  |  0110 0010   |       Label    |        None    |
        |  JLEZ    |  0x63  |  0110 0011   |       Label    |        None    |
        |  JCLEZ   |  0x64  |  0110 0100   |       Label    |        None    |
        |  CLF     |  0x80  |  1000 0000   |        None    |        None    |
 

## Version history
#### 0.
 1. First release with basic Gate classes, all with their respective unittests. 
   - Gates have a `std::bitset<n>` as input and a `std::bitset<1>` as output.
   - Operations are performed on each bit in the bitset (e.g. OR all bits in the bitset).
 2. Start of `MultiGate` implementation.
   - MultiGates have a list of `std::bitset<n>*` pointers so they can iterate every input a perform an operation on each input (e.g. OR entire bitset with the others).
 3. By using the [SynchrotronComponent class](https://github.com/Wosser1sProductions/Synchrotron) every `Gate` is now able to connects its output to inputs of other `Gates` and vica versa (*Signals & Slots* paradigm).
 4. Creating more components:
   - Added `SynchrotronComponentFixedInput`: with a fixed/minimum amount of inputs (e.g. `NOTGate` must have at most 1 input, `ANDGate` cannot take less than 2 inputs...)
   - Added `SHIFTLeft` and `SHIFTRight` to shift the connected signal by one to the left or right respectively.
   - Added [UML diagram](https://raw.githubusercontent.com/Wosser1sProductions/ScottyCPU/master/UML/ClassDiagram_0.4.42.png)
   - Added `Instruction` and `InstructionSet`
   - Added `Instruction` equivalent for every `CPUComponent`
   - Added `bitset` representations
   - Added `SCAMParser` and `SCAMAssembler` to parse and compile .ScAM files (Scotty Assembler Markup)
   - Fully implemented `ControlUnit`and `ScottyCPU`classes
 
#### Next
 - A `SynchrotronComponent` with an extra enable input (which can only be connected once)
 - Other `CPUComponents`
 - GUI connection with [Scotty Output GUI](https://github.com/llewella20/Scotty_Output_GUI)
 - Update Doxygen comments
