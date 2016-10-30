#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <bitset>

namespace CPUInstructions {

	/**
	 *	\brief	TO-DO
	 */
	enum class InstructionSet {
		/// Operators
		ADD, SUB, MUL, DIV, SHL, SHR,

		/// Compare
		CMP,

		/// Logic Operators
		NOT, AND, NAND, OR, NOR, XOR,

		/// Memory Operators
		MOV, DATA,

		/// Empty Instruction
		NOP,

		/// Jump Instructions (without condition)
		JMP,

		/// Jump if [E]qual, [L]arger, [S]maller, [Z]ero, [C]arry
		JE, JL, JS, JZ,
		JLE, JSE,
		JCE, JCL, JCS, JCZ,
		JLZ, JSZ, JEZ,
		JCLE, JCLZ, JCEZ, JLEZ, JCLEZ,

		/// Clear Jump Flags
		CLF
	};

	/**
	 *	\brief	TO-DO
	 */
	class Instruction {
		public:
			virtual void execute(void) = 0;
	};

}

#endif // INSTRUCTION_HPP
