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

	/**	\brief	A struct defining an assembled entry from a parsed ScAM file.
	 */
	typedef struct {
		size_t		LINENR;		///< The number of the line this entry will appear on when assembled.
		uint8_t		INSTR;		///< The binary representation of the Instruction.
		uint8_t		OPERANDS;	///< The binary representation of the Operands.
		bool		hasData;	///< True if this Instruction needs data (stored in the entry following this one; OPERANDS will be all 0's).
		bool		isData;		///< True if this entry contains data instead of an Instruction with Operands (data will be equal to `((INSTR << 8) | OPERANDS)`).
	} AssembledEntry;

	/**	\brief	A struct defining symbol entry from a parsed ScAM file (given by a label).
	 *
	 *		When re-inserting the contents of the SymbolTable in the assebbled code,<br>
	 *		a reference to a constant will be replaced with DATA on the following line,<br>
	 *		and a reference to an address will be replaced by LINENR on the following line.
	 */
	typedef struct {
		size_t		idx;		///< The original address where it appeared (for sorting/re-ordering purposes).
		size_t		LINENR;		///< The address where the symbol appears in the finished InstructionEntry table.
		bool		isConstant;	///< True if this is a constant value, false if its a label pointing to an Instruction.
		uint16_t	DATA;		///< The constant value if isConstant is true.
	} SymbolTableEntry;

	/**
	 *	\brief	**SCAMAssembler** :
	 *			An assembler for the `Scotty Assembler Markup` or `.ScAM` files associated with the ScottyCPU.
	 *
	 *		This assembler will create a SymbolTable and with this information,
	 *		assemble each entry in the InstructionEntry table to its corresponding machinecode after inserting the Symbols.
	 */
	//template <size_t bit_width>
	class SCAMAssembler : public SCAMParser {
		private:
			/**	\brief	The path to the output `.ScHex` file.
			 */
			std::string schexFile;

			/**	\brief	A list of Symbols appearing in the InstructionEntry table.
			 */
			std::unordered_map<std::string, CPUFactory::SymbolTableEntry*>	*symbol_table;

			/**	\brief	A list of elements of type AssembledEntry, used to for the assembled binary output.
			 */
			std::vector<CPUFactory::AssembledEntry*>						*assembled;

			/**	\brief	Parse the label of the given InstructionEntry and add it to the Symbol table.
			 *
			 *		Validity of this label was checked in SCAMParser::checkInstructionEntryLabel().
			 *
			 *	\param	cmd
			 *		The reference to the InstructionEntry where the label can be found.
			 *	\param	idx
			 *		The index in the InstructionEntry list were cmd occured.
			 *		<br>This is kept for re-ordering constant in a later stage.
			 */
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

			/**	\brief	Parse the given InstructionEntry and create an AssembledEntry in the AssembledEntry table.
			 *
			 *		Operand validity for this InstructionEntry was checked in SCAMParser::checkInstructionEntry().
			 *		<br>A valid Symbol table is needed to fill in constant values or addresses.
			 *		If a needed Symbol was not found, an error will be added to the parsing errors list,
			 *		and compilation (export to ScHex or printing) will not be possible.
			 *
			 *		- The strings in InstructionEntry will be cast to values with their proper sizes in AssembledEntry;
			 *		- CPUInstructions::getInstructionInfo() is used to get the machine code for each instruction;
			 *		- An extra AssembledEntry is added after Instructions that need extra data (e.g. full 16-bit address for jump instructions).
			 *
			 *	\param	cmd
			 *		The reference to the InstructionEntry to parse/compile/assemble.
			 */
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
							reg_num = SysUtils::lexical_cast<uint8_t>(cmd.OP1.substr(1).c_str());
							scam->OPERANDS |= (0xF0 & (reg_num << 4));

							switch(instr.Op2_type) {
								case CPUInstructions::OperandType::REG:
									reg_num = SysUtils::lexical_cast<uint8_t>(cmd.OP2.substr(1).c_str());
									scam->OPERANDS |= (0x0F & reg_num);
									break;
								case CPUInstructions::OperandType::VAL:
									scam->hasData = true;

									extra = SysUtils::lexical_cast<uint16_t>(cmd.OP2.c_str());

									data = SysUtils::allocVar<CPUFactory::AssembledEntry>();
									data->INSTR		= (extra & 0xFF00) >> 8;
									data->OPERANDS	= (extra & 0x00FF);

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
									data->INSTR		= (extra & 0xFF00) >> 8;
									data->OPERANDS	= (extra & 0x00FF);

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
							data->INSTR		= (extra & 0xFF00) >> 8;
							data->OPERANDS	= (extra & 0x00FF);

							switch(instr.Op2_type) {
								case CPUInstructions::OperandType::REG:
									reg_num = SysUtils::lexical_cast<uint8_t>(cmd.OP2.substr(1).c_str());
									scam->OPERANDS |= (0x0F & reg_num);
									break;
								case CPUInstructions::OperandType::VAL:
									extra = SysUtils::lexical_cast<uint16_t>(cmd.OP2.c_str());

									extra_data = SysUtils::allocVar<CPUFactory::AssembledEntry>();
									extra_data->LINENR		= cmd.LINENR + 1;
									extra_data->hasData		= false;
									extra_data->isData		= true;
									extra_data->INSTR		= (extra & 0xFF00) >> 8;
									extra_data->OPERANDS	= (extra & 0x00FF);

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
							data->INSTR		= (extra & 0xFF00) >> 8;
							data->OPERANDS	= (extra & 0x00FF);

							break;
						default:
							break;
					}
				} else {
					// Skip empty labeling or constants
					uint16_t data	= this->symbol_table->at(cmd.LBL)->DATA;
					scam->INSTR		= (data & 0xFF00) >> 8;
					scam->OPERANDS	= (data & 0x00FF);
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
			/**	\brief	Default constructor
			 *
			 *	\param	filename
			 *		The path to a `.ScAM` file to parse.
			 *
			 *	\exception	Exceptions::UnexpectedExtension
			 *		Throws UnexpectedExtension exception if the given file extension is not equal to SCAMParser::EXTENSION.
			 */
			SCAMAssembler(const std::string& filename)
				: SCAMParser(filename),
				  schexFile(std::strErasedFromLast(filename, ".") + SCAMAssembler::EXTENSION),
				  symbol_table(nullptr), assembled(nullptr) {}

			/**	\brief	Default destructor
			 */
			~SCAMAssembler() {
				if (this->symbol_table != nullptr)
					SysUtils::deallocMap(this->symbol_table);
				if (this->assembled != nullptr)
					SysUtils::deallocVector(this->assembled);
			}

			/**	\brief	Returns the path and name of the outputted binary file.
			 */
			const std::string& getFileName(void) const {
				return this->schexFile;
			}

			/**	\brief	Run the SCAMAssembler on this->scamFile.
			 *
			 *		1. **[PRE-TRANSLATE]**
			 *			<br>Iterate over each line in the InstructionEntry table and call parseSymbol()
			 *			<br>This will create a first version of the Symbol table.
			 *		2. **[REORDER CONSTANTS]**
			 *			<br>Get the position of the `MAIN` label from the Symbol table (required to be included by user),
			 *			<br>and check if any constants in the Symbol table appear after this label.
			 *			<br>If so, move them before `MAIN` (in the InstructionEntry table).
			 *			<br>This process can be seen by checking if the `Line` number differs (a lot) from the `Addr` in the InstructionEntry table.
			 *		3. **[DATA EXPANSION]**
			 *			<br>Check whether a function requires extra space in Memory (if it has 1 or 2 VAL operands or a LBL operand).
			 *			<br>If this is the case, increase the address of the next Instruction by the needed space.
			 *		4. **[REGENERATE]**
			 *			<br>Regenerate the Symbol table with the now corrected addresses.
			 *		5. **[ASSEMBLY]**
			 *			<br>Call parseCommand() on each InstructionEntry and assemble the Instruction to machine code.
			 *
			 *	\return	Returns whether the parsing produced any errors.
			 */
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
					for (CPUFactory::InstructionEntry* i : *this->getInstructionEntries()) {
						this->parseSymbol(*i, start++);
					}

					// Reorder constants to appear in beginning
					size_t main_entry_point = std::mapGetMapped(*this->symbol_table, "MAIN")->idx;

					for (std::pair<std::string const, CPUFactory::SymbolTableEntry*> i : *this->getSymbolEntries()) {
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

					for (CPUFactory::InstructionEntry* i : *this->getInstructionEntries()) {
						this->parseSymbol(*i, i->LINENR);
					}

					// Translate to machine code
					for (CPUFactory::InstructionEntry* i : *this->getInstructionEntries()) {
						this->parseCommand(*i);
					}
				}

				return !SCAMParser::hasErrors();
			}

			/**	\brief	Returns the SymbolTableEntry list.
			 *
			 *	**WARNING**	This will try to compile the file if the list is not yet available.
			 */
			const std::unordered_map<std::string, CPUFactory::SymbolTableEntry*>* getSymbolEntries(void) {
				if (this->symbol_table == nullptr) {
					this->compile();
				}

				return this->symbol_table;
			}

			/**	\brief	Returns the AssembledEntry list.
			 *
			 *	**WARNING**	This will try to compile the file if the list is not yet available.
			 */
			const std::vector<AssembledEntry*>* getAssembledEntries(void) {
				if (this->symbol_table == nullptr) {
					this->compile();
				}

				return this->assembled;
			}


			/**	\brief	Export the AssembledEntry table to a Scotty Hex file (`.ScHex`).
			 *
			 *	\param	filename
			 *		A new path and name for the exported file, if wanted.
			 *		<br>By default, a `.ScHex` is exported in the same directory as this->scamFile, but with the `.ScHex` extension instead.
			 */
			void exportScHex(const std::string& filename = "") {
				//std::string hexfile = this->tohex().toString();

				//this->schexFile = (filename.empty() ? std::strErasedFromLast(this->scamFile, ".") : filename) + SCAMAssembler::EXTENSION;

				if (this->hasErrors()) {
					std::cerr << std::endl << "There were errors while parsing the file,"
							  << std::endl << "please resolve them to be able to assemble it."
							  << std::endl;
				} else {
					if (!filename.empty())
						this->schexFile = filename;	// + SCAMAssembler::EXTENSION;

					size_t pos = 0;
					const size_t length = this->getAssembledEntries()->size() * 2;
					char *buffer = SysUtils::allocArray<char>(length);

					for (const CPUFactory::AssembledEntry* a : *this->getAssembledEntries()) {
						buffer[pos++] = a->INSTR;
						buffer[pos++] = a->OPERANDS;
					}

					SysUtils::writeBinaryFile(this->schexFile, buffer, length);

					SysUtils::deallocArray<char>(buffer);
				}
			}

			/**	\brief	Returns a string with the InstructionEntry table,
			 *			a SymbolTable, an AssembledEntry table and all parsing errors (if any).
			 */
			const std::string toString(void) {
				if (this->assembled == nullptr) {
					this->compile();
				}

				std::stringstream os;

				os << SCAMParser::toString();

				if (this->hasErrors()) {
					os << std::endl << "There were errors while parsing the file,"
					   << std::endl << "please resolve them to be able to assemble it."
					   << std::endl;
				} else {
					char *tmp = SysUtils::allocArray<char>(83);

					/**********************************************************************/
					/* Sort SymbolTable by re-inserting Pairs in std::set with custom sorter */
					typedef std::pair<std::string const, CPUFactory::SymbolTableEntry const*> Pair;

					struct linenr_sort {
							inline bool operator() (const Pair& lhs, const Pair& rhs) const {
								return lhs.second->LINENR <= rhs.second->LINENR;
							}
					};

					std::set<Pair, linenr_sort> sortedLUT(this->getSymbolEntries()->begin(), this->getSymbolEntries()->end());
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

					for (CPUFactory::AssembledEntry* a : *this->getAssembledEntries())
						os << SysUtils::stringFormat(tmp, 83, " %4d | 0x%02X%02X | %8s %8s |  %-5s  |  %-5s",
													 a->LINENR,
													 a->INSTR, a->OPERANDS,
													 std::bitset<8>(a->INSTR).to_string().c_str(),
													 std::bitset<8>(a->OPERANDS).to_string().c_str(),
													 a->hasData ? BOOLSTR(a->hasData) : "",
													 a->isData  ? BOOLSTR(a->isData)  : "")
						   << std::endl;

					SysUtils::deallocArray(tmp);
				}

				return os.str();
			}

			/**	\brief	Appends the output of toString() to the given stream.
			 *
			 *	\param	os
			 *		The stream to append the data to.
			 *	\param	s
			 *		A reference to the SCAMAssembler object to print the info from.
			 */
			friend std::ostream& operator<<(std::ostream &os, SCAMAssembler &s) {
				return os << s.toString();
			}

			/**	\brief	Appends the output of toString() to the given stream.
			 *
			 *	\param	os
			 *		The stream to append the data to.
			 *	\param	s
			 *		A pointer to the SCAMAssembler object to print the info from.
			 */
			friend std::ostream& operator<<(std::ostream &os, SCAMAssembler *s) {
				return os << *s;
			}

			/**	\brief	The default extenstion for a SCAMAssembler file: `.schex`.
			 */
			static const std::string EXTENSION;
	};

	/**	\brief	Set the default extenstion the SCAMAssembler to `.schex`.
	 */
	//template <size_t bit_width>
	const std::string SCAMAssembler/*<bit_width>*/::EXTENSION = ".schex";
}

#endif // SCAMASSEMBLER_HPP
