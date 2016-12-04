#ifndef SCAMASSEMBLER_HPP
#define SCAMASSEMBLER_HPP

#include <sstream>
#include <string>
#include <vector>

#include "SCAMParser.hpp"
#include "../CPUInstructions/Instruction.hpp"
#include "../utils.hpp"
#include "../Exceptions.hpp"

namespace CPUFactory {

	typedef struct {
		uint8_t		INSTR;
		uint8_t		FLAG;
		uint16_t	DATA;
	} AssembledEntry;

	typedef struct {
		size_t		LINENR;
		std::string	LBL;
	} SymbolTableEntry;

	template <size_t bit_width>
	class SCAMAssembler : public SCAMParser {
		protected:

			std::vector<CPUFactory::SymbolTableEntry*>	*symbol_table;

			std::vector<CPUFactory::AssembledEntry*>	*assembled;

			void parseCommand(const CPUFactory::InstructionEntry& cmd) {
				if (!cmd.LBL.empty()) {
				}
			}

		public:
			SCAMAssembler(const std::string& filename) : SCAMParser::SCAMParser(filename), symbol_table(nullptr), assembled(nullptr)  {
				this->symbol_table = SysUtils::allocVar<std::vector<CPUFactory::SymbolTableEntry*>>();
				this->assembled	= SysUtils::allocVar<std::vector<CPUFactory::AssembledEntry*>>();
			}

			~SCAMAssembler() {
				for (CPUFactory::SymbolTableEntry* i : *this->symbol_table) {
					SysUtils::deallocVar(i);
				}

				SysUtils::deallocVar(this->symbol_table);

				for (CPUFactory::AssembledEntry* i : *this->assembled) {
					SysUtils::deallocVar(i);
				}

				SysUtils::deallocVar(this->assembled);
			}

			void exportScHex(const std::string& filename = "") const {
				//std::string hexfile = this->tohex().toString();
				
				if (filename.empty()) {
					// SysUtils::writetofile(std::strEraseFrom(this->scamFile, ".") + SCAMAssembler::EXTENSION, hexfile);
				} else {
					// SysUtils::writetofile(filename + SCAMAssembler::EXTENSION, hexfile);
				}
			}
			
			static const std::string EXTENSION;
	};

	template <size_t bit_width>
	const std::string SCAMAssembler<bit_width>::EXTENSION = ".schex";
}

#endif // SCAMASSEMBLER_HPP
