#ifndef SCAMASSEMBLER_HPP
#define SCAMASSEMBLER_HPP

#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <bitset>

#include "SCAMParser.hpp"
#include "../CPUInstructions/Instruction.hpp"
#include "../utils.hpp"
#include "../Exceptions.hpp"

namespace CPUFactory {

	typedef struct {
		size_t		LINENR;
		uint8_t		INSTR;
		uint8_t		OPERANDS;
		bool		hasData;
		bool		isData;
	} AssembledEntry;

	typedef struct {
		size_t		idx;
		size_t		LINENR;
		bool		isConstant;
		uint16_t	DATA;
	} SymbolTableEntry;

	//template <size_t bit_width>
	class SCAMAssembler : public SCAMParser {
		protected:

			std::string schexFile;

			std::unordered_map<std::string, CPUFactory::SymbolTableEntry*> *symbol_table;

			std::vector<CPUFactory::AssembledEntry*> *assembled;

			void parseSymbol(const CPUFactory::InstructionEntry& cmd, size_t idx) {
				if (!cmd.LBL.empty()) {
					CPUFactory::SymbolTableEntry *symbol = SysUtils::allocVar<CPUFactory::SymbolTableEntry>();
					symbol->idx			= idx;
					symbol->LINENR		= cmd.LINENR;
					symbol->DATA		= 0;
					symbol->isConstant	= !cmd.NAME.empty() && !CPUInstructions::isValidInstruction(cmd.NAME);

					if (symbol->isConstant) {
						// Will cast alpha chars to 0, so cast checking might me redundant.
						symbol->DATA = SysUtils::lexical_cast<uint16_t>(cmd.NAME.c_str());
					}

					symbol_table->insert( { cmd.LBL, symbol } );
					//symbol_table->insert( std::make_pair<std::string, CPUFactory::SymbolTableEntry*>(cmd.LBL, symbol) );
					//symbol_table->insert( std::pair<std::string, CPUFactory::SymbolTableEntry*>(cmd.LBL, symbol) );
				}
			}

			void parseCommand(const CPUFactory::InstructionEntry& cmd) {
				bool isInstr = CPUInstructions::isValidInstruction(cmd.NAME);
				CPUFactory::AssembledEntry *scam = SysUtils::allocVar<CPUFactory::AssembledEntry>();
				CPUFactory::AssembledEntry *data = nullptr, *extra_data = nullptr;
				uint16_t extra = 0;

				scam->LINENR	= cmd.LINENR;
				scam->OPERANDS	= 0;
				scam->hasData	= false;
				scam->isData	= false;

				if (isInstr) {	// Compile instruction
					const CPUInstructions::InstructionInfo &instr = CPUInstructions::getInstructionInfo(cmd.NAME);
					uint8_t reg_num;

					scam->INSTR	= instr.OpCode;

					switch(instr.Op1_type) {
						case CPUInstructions::OperandType::REG:
							reg_num = SysUtils::lexical_cast<size_t>(cmd.OP1.substr(1).c_str());
							scam->OPERANDS |= (0xF0 & (reg_num << 4));

							switch(instr.Op2_type) {
								case CPUInstructions::OperandType::REG:
									reg_num = SysUtils::lexical_cast<size_t>(cmd.OP2.substr(1).c_str());
									scam->OPERANDS |= (0x0F & reg_num);
									break;
								case CPUInstructions::OperandType::VAL:
									scam->hasData = true;

									extra = SysUtils::lexical_cast<uint16_t>(cmd.OP2.c_str());

									data = SysUtils::allocVar<CPUFactory::AssembledEntry>();
									data->INSTR		= extra & 0xFF00;
									data->OPERANDS	= extra & 0x00FF;

									break;
								case CPUInstructions::OperandType::LBL:
									scam->hasData = true;

									extra = 0;

									if (std::mapHasKey(*this->symbol_table, cmd.OP2)) {
										extra = std::mapGetMapped(*this->symbol_table, cmd.OP2)->DATA;
									} else {
										InstructionEntryError *err = SysUtils::allocVar<CPUFactory::InstructionEntryError>();
										err->LBL = "ASSEMBLE ERROR";
										err->FILE_LINENR = cmd.FILE_LINENR;
										err->descr = "\"" + cmd.OP2 + "\" is not a known Symbol!\n\t\tCheck the SymbolTable for valid Symbols.";
										this->parse_errors->push_back(err);
									}

									data = SysUtils::allocVar<CPUFactory::AssembledEntry>();
									data->INSTR		= extra & 0xFF00;
									data->OPERANDS	= extra & 0x00FF;

									break;
								default:
									//case CPUInstructions::OperandType::NONE:
									break;
							}

							break;
						case CPUInstructions::OperandType::VAL:
							scam->hasData = true;

							extra = SysUtils::lexical_cast<uint16_t>(cmd.OP1.c_str());

							data = SysUtils::allocVar<CPUFactory::AssembledEntry>();
							data->INSTR		= extra & 0xFF00;
							data->OPERANDS	= extra & 0x00FF;

							switch(instr.Op2_type) {
								case CPUInstructions::OperandType::REG:
									reg_num = SysUtils::lexical_cast<size_t>(cmd.OP2.substr(1).c_str());
									scam->OPERANDS |= (0x0F & reg_num);
									break;
								case CPUInstructions::OperandType::VAL:
									extra = SysUtils::lexical_cast<size_t>(cmd.OP2.c_str());

									extra_data = SysUtils::allocVar<CPUFactory::AssembledEntry>();
									extra_data->LINENR		= cmd.LINENR + 1;
									extra_data->hasData		= false;
									extra_data->isData		= true;
									extra_data->INSTR		= extra & 0xFF00;
									extra_data->OPERANDS	= extra & 0x00FF;

									break;
								default:
									break;
							}

							break;
						case CPUInstructions::OperandType::LBL:
							scam->hasData = true;
							extra = 0;

							if (std::mapHasKey(*this->symbol_table, cmd.OP1)) {
								extra = std::mapGetMapped(*this->symbol_table, cmd.OP1)->LINENR;
							} else {
								InstructionEntryError *err = SysUtils::allocVar<CPUFactory::InstructionEntryError>();
								err->LBL = "ASSEMBLE ERROR";
								err->FILE_LINENR = cmd.FILE_LINENR;
								err->descr = "\"" + cmd.OP1 + "\" is not a known Symbol!\n\t\tCheck the SymbolTable for valid Symbols.";
								this->parse_errors->push_back(err);
							}

							data = SysUtils::allocVar<CPUFactory::AssembledEntry>();
							data->INSTR		= extra & 0xFF00;
							data->OPERANDS	= extra & 0x00FF;

							break;
						default:
							break;
					}
				} else {
					// Skip empty labeling or constants
					uint16_t data	= this->symbol_table->at(cmd.LBL)->DATA;
					scam->INSTR		= data & 0xFF00;
					scam->OPERANDS	= data & 0x00FF;
				}

				this->assembled->push_back(scam);
				if (data != nullptr) {
					data->LINENR	= cmd.LINENR + 1;
					data->hasData	= false;
					data->isData	= true;
					this->assembled->push_back(data);
				}
				if (extra_data != nullptr)
					this->assembled->push_back(extra_data);
			}

		public:
			SCAMAssembler(const std::string& filename)
				: SCAMParser(filename),
				  schexFile(std::strErasedFrom(filename, ".") + SCAMAssembler::EXTENSION),
				  symbol_table(nullptr), assembled(nullptr) {}

			~SCAMAssembler() {
				if (this->symbol_table != nullptr)
					SysUtils::deallocMap(this->symbol_table);
				if (this->assembled != nullptr)
					SysUtils::deallocVector(this->assembled);
			}

			const std::string& getFileName(void) const {
				return this->schexFile;
			}

			bool compile(void) {
				if (this->assembled != nullptr) {
					SysUtils::deallocMap(this->symbol_table);
					SysUtils::deallocVector(this->assembled);
				}

				this->reparseFile();

				if (!SCAMParser::hasErrors()) {

					this->symbol_table = SysUtils::allocVar<std::unordered_map<std::string, CPUFactory::SymbolTableEntry*>>();
					this->assembled	   = SysUtils::allocVar<std::vector<CPUFactory::AssembledEntry*>>();
					size_t start = 0;

					// Pre-translate : Create Symbol Table
					for (CPUFactory::InstructionEntry* i : this->getInstructionEntries()) {
						this->parseSymbol(*i, start++);
					}

					// Reorder constants to appear in beginning
					size_t main_entry_point = std::mapGetMapped(*this->symbol_table, "MAIN")->idx;

					for (std::pair<std::string const, CPUFactory::SymbolTableEntry*> i : this->getSymbolEntries()) {
						if (i.second->isConstant && i.second->LINENR >= main_entry_point) {
							start = i.second->idx;
							CPUFactory::InstructionEntry* tmp = this->instr_entries->at(start);
							tmp->LINENR = main_entry_point+1;

							this->instr_entries->erase(this->instr_entries->begin() + start,
													   this->instr_entries->begin() + start + 1);
							this->instr_entries->insert(this->instr_entries->begin() + main_entry_point++,
														tmp);
						}
					}

					// Expand LINENR to compensate for DATA to be added
					start = 0;
					for (CPUFactory::InstructionEntry* i : *this->instr_entries) {
						i->LINENR = start++;

						if (CPUInstructions::isValidInstruction(i->NAME)) {
							const CPUInstructions::InstructionInfo &instr = CPUInstructions::getInstructionInfo(i->NAME);

							if (	(instr.Op1_type == CPUInstructions::OperandType::REG
									&& (   instr.Op2_type == CPUInstructions::OperandType::VAL
										|| instr.Op2_type == CPUInstructions::OperandType::LBL))
								||	(instr.Op1_type == CPUInstructions::OperandType::VAL)
								||	(instr.Op1_type ==CPUInstructions::OperandType::LBL))
								++start;
							if (	(instr.Op1_type == CPUInstructions::OperandType::VAL)
								&&	(instr.Op2_type == CPUInstructions::OperandType::VAL))
								++start;
						}
					}

					// Regenerate SymbolTable with updated addresses
					SysUtils::deallocMap(this->symbol_table);
					this->symbol_table = SysUtils::allocVar<std::unordered_map<std::string, CPUFactory::SymbolTableEntry*>>();

					for (CPUFactory::InstructionEntry* i : this->getInstructionEntries()) {
						this->parseSymbol(*i, i->LINENR);
					}

					// Translate to machine code
					for (CPUFactory::InstructionEntry* i : this->getInstructionEntries()) {
						this->parseCommand(*i);
					}
				}

				return !SCAMParser::hasErrors();
			}

			const std::unordered_map<std::string, CPUFactory::SymbolTableEntry*>& getSymbolEntries(void) {
				if (this->symbol_table == nullptr) {
					this->compile();
				}

				return *this->symbol_table;
			}

			const std::vector<AssembledEntry*>& getAssembledEntries(void) {
				if (this->symbol_table == nullptr) {
					this->compile();
				}

				return *this->assembled;
			}

			void exportScHex(const std::string& filename = "") {
				//std::string hexfile = this->tohex().toString();

				this->schexFile = (filename.empty() ? std::strErasedFrom(this->scamFile, ".") : filename) + SCAMAssembler::EXTENSION;
				size_t pos = 0;
				const size_t length = this->getAssembledEntries().size() * 2;
				char *buffer = SysUtils::allocArray<char>(length);

				for (const CPUFactory::AssembledEntry* a : this->getAssembledEntries()) {
					buffer[pos++] = a->INSTR;
					buffer[pos++] = a->OPERANDS;
				}

				SysUtils::writeBinaryFile(this->schexFile, buffer, length);

				SysUtils::deallocArray<char>(buffer);
			}

			const std::string toString(void) {
				if (this->assembled == nullptr) {
					this->compile();
				}

				std::stringstream os;
				char *tmp = SysUtils::allocArray<char>(83);

				os << SCAMParser::toString();

				/**********************************************************************/
				/* Sort InstructionLUT by re-inserting Pairs in std::set with custom sorter */
				typedef std::pair<std::string const, CPUFactory::SymbolTableEntry const*> Pair;

				struct linenr_sort {
					inline bool operator() (const Pair& lhs, const Pair& rhs) const {
						return lhs.second->LINENR <= rhs.second->LINENR;
					}
				};

				std::set<Pair, linenr_sort> sortedLUT(this->getSymbolEntries().begin(), this->getSymbolEntries().end());
				/****************************************************************************/

				os << std::endl << "SCAM-file: Symboltable" << std::endl;
				os << " Addr |    Label    | isConstant |  Value  | Decimal"  << std::endl;
				os << "------+-------------+------------+---------+---------" << std::endl;

				for (const Pair& s : sortedLUT)
					os << SysUtils::stringFormat(tmp, 83, " %4d | %-11s |   %-5s    |  0x%04X | % 7d",
												 s.second->LINENR, s.first.c_str(), BOOLSTR(s.second->isConstant), s.second->DATA, s.second->DATA)
					   << std::endl;

				os << std::endl << "SCAM-file: Assembled" << std::endl;
				os << " Addr |  ScAM  | Instr    Operand  | hasData | isData" << std::endl;
				os << "------+--------+-------------------+---------+--------" << std::endl;

				for (CPUFactory::AssembledEntry* a : this->getAssembledEntries())
					os << SysUtils::stringFormat(tmp, 83, " %4d | 0x%02X%02X | %8s %8s |  %-5s  |  %-5s",
												 a->LINENR,
												 a->INSTR, a->OPERANDS,
												 std::bitset<8>(a->INSTR).to_string().c_str(),
												 std::bitset<8>(a->OPERANDS).to_string().c_str(),
												 a->hasData ? BOOLSTR(a->hasData) : "",
												 a->isData  ? BOOLSTR(a->isData)  : "")
					   << std::endl;

				SysUtils::deallocArray(tmp);

				return os.str();
			}

			friend std::ostream& operator<<(std::ostream &os, SCAMAssembler &s) {
				return os << s.toString();
			}

			friend std::ostream& operator<<(std::ostream &os, SCAMAssembler *s) {
				return os << *s;
			}

			static const std::string EXTENSION;
	};

	//template <size_t bit_width>
	const std::string SCAMAssembler/*<bit_width>*/::EXTENSION = ".schex";
}

#endif // SCAMASSEMBLER_HPP
