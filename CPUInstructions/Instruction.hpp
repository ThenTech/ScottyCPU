#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <unordered_map>
#include <bitset>

#define I(X)	((size_t) X)

//#include "../SynchrotronComponent.hpp"
//using namespace Synchrotron;

namespace CPUInstructions {

	/**
	 *	\brief	The complete instruction set.
	 */
	enum class InstructionSet : size_t {
		/// Operators
		ADD		= 0x01,  // 0000 0001
		SUB		= 0x02,  // 0000 0010
		MUL		= 0x03,  // 0000 0011
		DIV		= 0x04,  // 0000 0100
		MOD		= 0x05,  // 0000 0101
		SHL		= 0x06,  // 0000 0110
		SHR		= 0x07,  // 0000 0111
		// Extra? ROtateRight, ROtateLeft
		//ROR= 0x08,  // 0000 1000
		//ROL= 0x09,  // 0000 1001

		/// Compare
		CMP		= 0x1F,  // 0001 1111

		/// Logic Operators
		NOT		= 0x11,  // 0001 0001
		AND		= 0x12,  // 0001 0010
		NAND	= 0x13,  // 0001 0011
		OR		= 0x14,  // 0001 0100
		NOR		= 0x15,  // 0001 0101
		XOR		= 0x16,  // 0001 0110

		/// Memory Operators
		MOV		= 0x20,  // 0010 0000
		DATA	= 0x21,  // 0010 0001

		/// Empty Instruction
		NOP		= 0x00,  // 0000 0000

		/// Jump Instructions (without condition)
		JMP		= 0x40,  // 0100 0000

		/// Jump if [E]qual, [L]arger, [S]maller, [Z]ero, [C]arry
		JE		= 0x41,  // 0100 0001
		JL		= 0x42,  // 0100 0010
		JS		= 0x43,  // 0100 0011
		JZ		= 0x44,  // 0100 0100

		JLE		= 0x50,  // 0101 0000
		JSE		= 0x51,  // 0101 0001

		JCE		= 0x52,  // 0101 0010
		JCL		= 0x53,  // 0101 0011
		JCS		= 0x54,  // 0101 0100
		JCZ		= 0x55,  // 0101 0101

		JLZ		= 0x56,  // 0101 0110
		JSZ		= 0x57,  // 0101 0111
		JEZ		= 0x58,  // 0101 1000

		JCLE	= 0x60,  // 0110 0000
		JCLZ	= 0x61,  // 0110 0001
		JCEZ	= 0x62,  // 0110 0010
		JLEZ	= 0x63,  // 0110 0011
		JCLEZ	= 0x64,  // 0110 0100

		/// Clear Jump Flags
		CLF		= 0x80,  // 1000 0000
	};

	static const std::unordered_map<std::string, size_t> InstructionLUT =
	{
		{ "ADD",	I(InstructionSet::ADD)		},
		{ "SUB",	I(InstructionSet::SUB)		},
		{ "MUL",	I(InstructionSet::MUL)		},
		{ "DIV",	I(InstructionSet::DIV)		},
		{ "MOD",	I(InstructionSet::MOD)		},
		{ "SHL",	I(InstructionSet::SHL)		},
		{ "SHR",	I(InstructionSet::SHR)		},

		{ "CMP",	I(InstructionSet::CMP)		},

		{ "NOT",	I(InstructionSet::NOT)		},
		{ "AND",	I(InstructionSet::AND)		},
		{ "NAND",	I(InstructionSet::NAND)		},
		{ "OR",		I(InstructionSet::OR)		},
		{ "NOR",	I(InstructionSet::NOR)		},
		{ "XOR",	I(InstructionSet::XOR)		},

		{ "MOV",	I(InstructionSet::MOV)		},
		{ "DATA",	I(InstructionSet::DATA)		},

		{ "NOP",	I(InstructionSet::NOP)		},

		{ "JMP",	I(InstructionSet::JMP)		},

		{ "JE",		I(InstructionSet::JE)		},
		{ "JL",		I(InstructionSet::JL)		},
		{ "JS",		I(InstructionSet::JS)		},
		{ "JZ",		I(InstructionSet::JZ)		},

		{ "JLE",	I(InstructionSet::JLE)		},
		{ "JSE",	I(InstructionSet::JSE)		},

		{ "JCE",	I(InstructionSet::JCE)		},
		{ "JCL",	I(InstructionSet::JCL)		},
		{ "JCS",	I(InstructionSet::JCS)		},
		{ "JCZ",	I(InstructionSet::JCZ)		},

		{ "JLZ",	I(InstructionSet::JLZ)		},
		{ "JSZ",	I(InstructionSet::JSZ)		},
		{ "JEZ",	I(InstructionSet::JEZ)		},

		{ "JCLE",	I(InstructionSet::JCLE)		},
		{ "JCLZ",	I(InstructionSet::JCLZ)		},
		{ "JCEZ",	I(InstructionSet::JCEZ)		},
		{ "JLEZ",	I(InstructionSet::JLEZ)		},
		{ "JCLEZ",	I(InstructionSet::JCLEZ)	},

		{ "CLF",	I(InstructionSet::CLF)		}
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
		FLAGS_COUNT = I(FLAGS::DivByZero)
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
			std::bitset<(size_t) FLAGS::FLAGS_COUNT> REG_FLAGS;
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
			const std::bitset<(size_t) FLAGS::FLAGS_COUNT>& getFlags(void) const {
				return this->REG_FLAGS;
			}
	};
}

#endif // INSTRUCTION_HPP
