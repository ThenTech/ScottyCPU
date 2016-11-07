#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "../SynchrotronComponent.hpp"
using namespace Synchrotron;

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
	enum class FLAGS {
		CLEAR		= 0x00,
		Smaller		= 0x01,
		Equal		= 0x02,
		Larger		= 0x04,
		Zero		= 0x08,
		CarryOut	= 0x10,
		FLAGS_COUNT = 0x05
	};

//	typedef struct {
//		bool Smaller, Equal, Larger, Zero, CarryOut;
//	} Flags;

	/**
	 *	\brief	TO-DO
	 */
	class Instruction {
		private:
			//Flags REG_FLAGS;
			std::bitset<(size_t) FLAGS::FLAGS_COUNT>	REG_FLAGS;
			//SynchrotronComponent<FLAGS::FLAGS_COUNT> REG_FLAGS;

		protected:

			void setFlag(FLAGS f) {
				if (f == FLAGS::CLEAR)	this->REG_FLAGS.reset();
				else					this->REG_FLAGS |= std::bitset<5>((size_t) f);
			}

		public:

			Instruction()
				: /* REG_FLAGS( { false } ) */
				  REG_FLAGS((size_t) FLAGS::CLEAR)
				  /* REG_FLAGS() */
			{
				// Empty
			}

			~Instruction() {}

//			virtual void execute(void) = 0;

			bool flagIsSet(FLAGS f) const {
				return this->REG_FLAGS.test((size_t) f);
			}

			std::bitset<(size_t) FLAGS::FLAGS_COUNT>& getFlags(void) {
				return this->REG_FLAGS;
			}

//			/// From Synchrotron
//			inline void tick(void) {
//				SynchrotronComponent<bit_width>::tick();
//				// Check for changes after operation and setFlag() where necessary.
//			}
	};


#include "../ANDGate.hpp"
	/**
	 *	\brief	TO-DO
	 */
	template <size_t bit_width>
	class ANDInstruction : public CPUComponents::ANDGate<bit_width>, public Instruction {
		private:

		public:
			ANDInstruction() : CPUComponents::ANDGate<bit_width>(0u) {}
				
			~ANDInstruction() {}
			
			inline void tick() {
				CPUComponents::ANDGate<bit_width>::tick();

				if (this->state.none())
					this->setFlag(FLAGS::Zero);

				//if (prevState != this->state) this->emit();
			}
	};

}

#endif // INSTRUCTION_HPP
