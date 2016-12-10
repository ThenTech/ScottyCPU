#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <unordered_map>
#include <set>
#include <bitset>
#include "../utils.hpp"

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
		MOVRR	= 0x20,  // 0010 0000
		MOVRA	= 0x22,  // 0010 0010
		MOVAR	= 0x24,  // 0010 0100
		MOVAA	= 0x28,  // 0010 1000

		DATA	= 0x31,  // 0011 0001
		DATAC	= 0x32,  // 0011 0010

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

		/// Bitsize of one instruction
		LENGHT	= 0x08,  // 0000 1000
	};

	enum class OperandType {
		REG, VAL, LBL, NONE
	};

	static inline std::string OperandTypeToString(OperandType u) {
		static std::string LUT[] = { "Register", "Value", "Label", "None" };
		return LUT[UINT(u)];
	}

	typedef struct {
		size_t		OpCode;
		OperandType Op1_type;
		OperandType Op2_type;
		//size_t		extra_bit_width_size;	// MOV with values
	} InstructionInfo;

//	static const std::unordered_map<std::string, size_t> RegisterLUT =
//	{
//		{ "R0" , 0 }, { "R1" , 1 }, { "R2" , 2 }, { "R3" , 3 }, { "R4" , 4 },
//		{ "R5" , 5 }, { "R6" , 5 }, { "R7" , 7 }, { "R8" , 8 }, { "R9" , 9 }
//	};

	/// Bitsize of one register
	static const size_t REG_BITS_SIZE = 4;

	/// Register prefix
	static const std::string REG_PREFIX = "R$";

	// bitsize of 1 instruction = UINT(InstructionSet::LENGHT) << 1 + (amount of OperandType::VALs in optype) * 16

	static const std::unordered_map<std::string, InstructionInfo> InstructionLUT =
	{
		{ "ADD",	{ UINT(InstructionSet::ADD),	OperandType::REG, OperandType::REG	} },
		{ "SUB",	{ UINT(InstructionSet::SUB),	OperandType::REG, OperandType::REG	} },
		{ "MUL",	{ UINT(InstructionSet::MUL),	OperandType::REG, OperandType::REG	} },
		{ "DIV",	{ UINT(InstructionSet::DIV),	OperandType::REG, OperandType::REG	} },
		{ "MOD",	{ UINT(InstructionSet::MOD),	OperandType::REG, OperandType::REG	} },
		{ "SHL",	{ UINT(InstructionSet::SHL),	OperandType::REG, OperandType::NONE	} },
		{ "SHR",	{ UINT(InstructionSet::SHR),	OperandType::REG, OperandType::NONE	} },

		{ "CMP",	{ UINT(InstructionSet::CMP),	OperandType::REG, OperandType::REG	} },

		{ "NOT",	{ UINT(InstructionSet::NOT),	OperandType::REG, OperandType::NONE	} },
		{ "AND",	{ UINT(InstructionSet::AND),	OperandType::REG, OperandType::REG	} },
		{ "NAND",	{ UINT(InstructionSet::NAND),	OperandType::REG, OperandType::REG	} },
		{ "OR",		{ UINT(InstructionSet::OR),		OperandType::REG, OperandType::REG	} },
		{ "NOR",	{ UINT(InstructionSet::NOR),	OperandType::REG, OperandType::REG	} },
		{ "XOR",	{ UINT(InstructionSet::XOR),	OperandType::REG, OperandType::REG	} },

		{ "MOV",	{ UINT(InstructionSet::MOV),	OperandType::REG, OperandType::REG,	} },
		{ "MOVRR",	{ UINT(InstructionSet::MOVRR),	OperandType::REG, OperandType::REG,	} },
		{ "MOVRA",	{ UINT(InstructionSet::MOVRA),	OperandType::REG, OperandType::VAL,	} }, // +bit_width
		{ "MOVAR",	{ UINT(InstructionSet::MOVAR),	OperandType::VAL, OperandType::REG,	} }, // +bit_width
		{ "MOVAA",	{ UINT(InstructionSet::MOVAA),	OperandType::VAL, OperandType::VAL,	} }, // +bit_width<<1

		{ "DATA",	{ UINT(InstructionSet::DATA),	OperandType::REG, OperandType::VAL,	} },
		{ "DATAC",	{ UINT(InstructionSet::DATAC),	OperandType::REG, OperandType::LBL,	} },

		{ "NOP",	{ UINT(InstructionSet::NOP),	OperandType::NONE, OperandType::NONE} },

		{ "JMP",	{ UINT(InstructionSet::JMP),	OperandType::LBL, OperandType::NONE	} },

		{ "JE",		{ UINT(InstructionSet::JE),		OperandType::LBL, OperandType::NONE	} },
		{ "JL",		{ UINT(InstructionSet::JL),		OperandType::LBL, OperandType::NONE	} },
		{ "JS",		{ UINT(InstructionSet::JS),		OperandType::LBL, OperandType::NONE	} },
		{ "JZ",		{ UINT(InstructionSet::JZ),		OperandType::LBL, OperandType::NONE	} },

		{ "JLE",	{ UINT(InstructionSet::JLE),	OperandType::LBL, OperandType::NONE	} },
		{ "JSE",	{ UINT(InstructionSet::JSE),	OperandType::LBL, OperandType::NONE	} },

		{ "JCE",	{ UINT(InstructionSet::JCE),	OperandType::LBL, OperandType::NONE	} },
		{ "JCL",	{ UINT(InstructionSet::JCL),	OperandType::LBL, OperandType::NONE	} },
		{ "JCS",	{ UINT(InstructionSet::JCS),	OperandType::LBL, OperandType::NONE	} },
		{ "JCZ",	{ UINT(InstructionSet::JCZ),	OperandType::LBL, OperandType::NONE	} },

		{ "JLZ",	{ UINT(InstructionSet::JLZ),	OperandType::LBL, OperandType::NONE	} },
		{ "JSZ",	{ UINT(InstructionSet::JSZ),	OperandType::LBL, OperandType::NONE	} },
		{ "JEZ",	{ UINT(InstructionSet::JEZ),	OperandType::LBL, OperandType::NONE	} },

		{ "JCLE",	{ UINT(InstructionSet::JCLE),	OperandType::LBL, OperandType::NONE	} },
		{ "JCLZ",	{ UINT(InstructionSet::JCLZ),	OperandType::LBL, OperandType::NONE	} },
		{ "JCEZ",	{ UINT(InstructionSet::JCEZ),	OperandType::LBL, OperandType::NONE	} },
		{ "JLEZ",	{ UINT(InstructionSet::JLEZ),	OperandType::LBL, OperandType::NONE	} },
		{ "JCLEZ",	{ UINT(InstructionSet::JCLEZ),	OperandType::LBL, OperandType::NONE	} },

		{ "CLF",	{ UINT(InstructionSet::CLF),	OperandType::NONE, OperandType::NONE} }
	};

	/** TO-DO
	 *
		std::ostream& operator<<(std::ostream &os, const std::unordered_map<std::string, InstructionInfo>& lut) {
			// os << printInstructionSet()
			return os;
		}
	 */
	static void printInstructionSet(void) {
		char *tmp = SysUtils::allocArray<char>(83);
		std::cout << "/----------------------------------------------------------------------------\\" << std::endl
				  << "|------------------------- ScottyCPU InstructionSet -------------------------|"  << std::endl
				  << "\\----------------------------------------------------------------------------/" << std::endl << std::endl
				  << "    |   Name   | OpCode | Operand code | Operand 1 type | Operand 2 type |"	<< std::endl
				  << "    +----------+--------+--------------+----------------+----------------+" << std::endl;;

		/**********************************************************************/
		/* Sort InstructionLUT by re-inserting Pairs in std::set with custom sorter */
		typedef std::pair<std::string const, CPUInstructions::InstructionInfo const> Pair;

		struct opcode_sort {
			inline bool operator() (const Pair& lhs, const Pair& rhs) const {
				return lhs.second.OpCode <= rhs.second.OpCode;
			}
		};

		std::set<Pair, opcode_sort> sortedLUT(InstructionLUT.begin(), InstructionLUT.end());
		/****************************************************************************/

		for (const Pair& i : sortedLUT) {
			std::cout << SysUtils::stringFormat(tmp, 83, "    |  %-5s   |  0x%02X  |  %04s %04s   |    %8s    |    %8s    |",
												i.first.c_str(),
												i.second.OpCode,
												std::bitset<4>((i.second.OpCode & 0xF0) >> 4).to_string().c_str(),
												std::bitset<4>(i.second.OpCode & 0x0F).to_string().c_str(),
												OperandTypeToString(i.second.Op1_type).c_str(),
												OperandTypeToString(i.second.Op2_type).c_str())
					  << std::endl;
		}

		SysUtils::deallocArray(tmp);
	}

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
		FLAGS_COUNT = UINT(FLAGS::DivByZero)
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
			std::bitset<UINT(FLAGS::FLAGS_COUNT)> REG_FLAGS;
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
				else					this->REG_FLAGS.set(UINT(f) - 1);
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
			FlagRegister(std::bitset<UINT(FLAGS::FLAGS_COUNT)> __val = UINT(FLAGS::CLEAR))
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
				return this->REG_FLAGS.test(UINT(f) - 1);
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
