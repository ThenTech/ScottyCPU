# [ScottyCPU](http://scottycpufor.me/) [![Build status](https://ci.appveyor.com/api/projects/status/0exw7bal3h8n53cp?svg=true)](https://ci.appveyor.com/project/Wosser1sProductions/scottycpu) [![Issue Count](https://codeclimate.com/github/Wosser1sProductions/ScottyCPU/badges/issue_count.svg)](https://codeclimate.com/github/Wosser1sProductions/ScottyCPU) [![GitHub issues](https://img.shields.io/github/issues/Wosser1sProductions/ScottyCPU.svg)](https://github.com/Wosser1sProductions/ScottyCPU/issues) [![DrMemory errors](https://img.shields.io/badge/DrMemory-0_errors-brightgreen.svg)](http://www.drmemory.org/) [![progress](https://img.shields.io/badge/total%20progress-70%25-yellowgreen.svg)](http://doc.scottycpufor.me)

The base code for a "Scotty Processor" as described by *J. Clark Scott* in his book [But How Do It Know?](http://www.buthowdoitknow.com/).

> But How Do It Know? is the breakthrough book that distills Computer Science down to the simple basic principles that 
all computers have in common, and presents the big picture step by step in plain English so that anyone can understand 
this ingenious, yet basically simple invention called a computer.


For an upcomig school project, we will try to create this CPU model for educational purposes with:
 - Complete functionality regarding a simple processor system
 - Providing visual information when running assembly programs on the CPU (e.g. memory dumps, step-by-step execution...)
 
By using C++ and eventually the Qt framework for a GUI.

Documentation has been generated with [Doxygen](http://www.doxygen.org/index.html) and can be found [here](http://doc.scottycpufor.me/) or as an archive in the `release` section.

This batchfile can be used to copy files from working dir to GitHub dir for committing: [gitCPTY.bat](https://gist.github.com/Wosser1sProductions/638ff2219b8786a83df863c574a8e58d)

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
   - Added UML diagram
   - Added `Instruction` and `InstructionSet`
   - Added `Instruction` equivalent for every `CPUComponent`
   - Added `bitset` representations
   - Added `SCAMParser` and `SCAMAssembler` to parse and compile .ScAM files (Scotty Assembler Markup)
 
#### Next
Adding additional implementations of `SynchrotronComponent` for extra functionality:
 - A `SynchrotronComponent` with an extra enable input (which can only be connected once)
 - Other `CPUComponents`
 - Functioning `ControlUnit`
 - GUI connection with [Scotty Output GUI](https://github.com/llewella20/Scotty_Output_GUI)
