#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <bitset>

//#include "../SynchrotronComponent.hpp"
//using namespace Synchrotron;

namespace CPUInstructions {

	/**
	 *	\brief	The complete instruction set.
	 */
	enum class InstructionSet {
		/// Operators
		ADD, SUB, MUL, DIV, MOD, SHL, SHR,
		// Extra? ROtateRight, ROtateLeft

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
	 *	\brief	Enum containing all flags possible in the CPU.
	 */
	enum class FLAGS {
		CLEAR		= 0,
		Smaller		= 1,
		Equal		= 2,
		Larger		= 3,
		Zero		= 4,
		Negative	= 5,
		CarryOut	= 6,
		DivByZero	= 7,
		FLAGS_COUNT = FLAGS::DivByZero
	};

//	typedef struct {
//		bool Smaller, Equal, Larger, Zero, CarryOut;
//	} Flags;

	/**
	 *	\brief	**FlagRegister** : Get and set flags after operations.
	 */
	class FlagRegister {
		private:
			/**
			 *	\brief	Bitset containing flags on bit location as set in FLAGS.
			 */
			//Flags REG_FLAGS;
			std::bitset<(size_t) FLAGS::FLAGS_COUNT>	REG_FLAGS;
			//SynchrotronComponent<FLAGS::FLAGS_COUNT> REG_FLAGS;

		protected:
			/**
			 *	\brief	Clear the current flags.
			 */
			inline void clearFlags() {
				this->REG_FLAGS.reset();
			}

			/**
			 *	\brief	Set a flag.
			 *
			 *	\param	f
			 *		The flag to set (from the FLAGS enum).
			 */
			inline void setFlag(FLAGS f) {
				if (f == FLAGS::CLEAR)	this->clearFlags();
				else					this->REG_FLAGS.set(((size_t) f) - 1);
			}

			/**
			 *	\brief	Set a flag if result is Zero of Negative (output from Logic Gates).
			 *
			 *	\param	result
			 *		Bitset to check for setting flags.
			 */
			template <size_t bit_width>
			inline void evaluateResultConditions(std::bitset<bit_width> const& result) {
				if (result.none())
					this->setFlag(FLAGS::Zero);
				if (result.test(bit_width - 1))
					this->setFlag(FLAGS::Negative);
			}

			/**
			 *	\brief	Returns a bitmask of size bit_width with the MSB set (to check for CarryOut).
			 */
			template <size_t bit_width>
			inline unsigned long long int getMSBMask(void) const {
				static const unsigned long long int MSB_MASK = (1 << bit_width);
				return MSB_MASK;
			}

		public:
			/**
			 *	Default constructor
			 *
			 *	\param	__val
			 *			Default value to set the flags.
			 */
			FlagRegister(std::bitset<(size_t) FLAGS::FLAGS_COUNT> __val = (size_t) FLAGS::CLEAR)
				: REG_FLAGS(__val) /* REG_FLAGS() */ {}

			/**
			 *	Default destructor
			 */
			~FlagRegister() {}

			/**
			 *	\brief	Check if a flag is set.
			 *
			 *	\param	f
			 *		The flag to check for.
			 *
			 *	\return	bool
			 *		Returns true if the given flag is set.
			 */
			inline bool flagIsSet(FLAGS f) const {
				if (f == FLAGS::CLEAR)
					return this->REG_FLAGS.none();
				return this->REG_FLAGS.test(((size_t) f) - 1);
			}

			/**
			 *	\brief	Returns the flag register.
			 */
			const std::bitset<(size_t) FLAGS::FLAGS_COUNT>& getFlags(void) {
				return this->REG_FLAGS;
			}
	};
}

#endif // INSTRUCTION_HPP
