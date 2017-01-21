#ifndef SCAMPARSER_HPP
#define SCAMPARSER_HPP

#include <sstream>
#include <string>
#include <vector>

#include "../CPUInstructions/Instruction.hpp"
#include "../utils.hpp"
#include "../Exceptions.hpp"

namespace CPUFactory {

	/**	\brief	A struct defining an entry from a parsed ScAM file.
	 */
	typedef struct {
		size_t		FILE_LINENR;	///< The number of the line this entry appears on in the parsed file.
		size_t		LINENR;			///< The number of the line this entry will appear on when compiled.
		std::string LBL;			///< The label found on this line.
		std::string NAME;			///< The instruction name found on this line.
		std::string OP1;			///< The first operand found on this line.
		std::string OP2;			///< The second operand found on this line.
	} InstructionEntry;

	/**	\brief	A struct defining an entry in the parse errors list.
	 */
	typedef struct {
		size_t		FILE_LINENR;	///< The number of the line this error appears on in the parsed file.
		std::string LBL;			///< The label of the error.
		std::string descr;			///< The description of the error.
	} InstructionEntryError;


	/**
	 *	\brief	**SCAMParser** :
	 *			A parser for the `Scotty Assembler Markup` or `.ScAM` files associated with the ScottyCPU.
	 *
	 *		This parser will check a given `.ScAM` file on type errors regarding the used
	 *		mnemonic names for Instructions and their operands.
	 */
	class SCAMParser {
		protected:
			/**	\brief	The path to the `.ScAM` file to parse.
			 */
			const std::string scamFile;

			/**	\brief	The raw string read from the this->scamFile.
			 */
			const std::string *rawFileData;

			/**	\brief	A list of elements of type CPUFactory::InstructionEntry.
			 *			Each element represents a non-comment line from the parsed file;
			 *			that potentially has executable code.
			 */
			std::vector<CPUFactory::InstructionEntry*>		*instr_entries;

			/**	\brief	A list of elements of type CPUFactory::InstructionEntryError.
			 *			Each element represents an error found while parsing the file.
			 */
			std::vector<CPUFactory::InstructionEntryError*> *parse_errors;

		private:
			/**	\brief	Check if a given label is valid (in reference to its InstructionEntry).
			 *
			 *		A label is valid if it:
			 *
			 *		- Does not start with a CPUInstructions::REG_PREFIX character ('R' or '$');
			 *		- Does not have whitespace characters;
			 *		- Does not start with a digit.
			 *
			 *		If any of these conditions is not met, an entry will be added to the parsing error list.
			 *
			 *	\param	i
			 *		The reference to the InstructionEntry where the label could be found.
			 *	\param	lbl
			 *		The label to check.
			 */
			void checkInstructionEntryLabel(const CPUFactory::InstructionEntry &i, const std::string &lbl) {
				bool startsReg   = std::strHasChar(CPUInstructions::REG_PREFIX, lbl[0]),
					 hasSpace    = std::strHasChar(lbl, ' ') || std::strHasChar(lbl, '\t'),
					 startsDigit = lbl[0] >= '0' && lbl[0] <= '9';

				if (startsReg || hasSpace || startsDigit) {
					InstructionEntryError *err = SysUtils::allocVar<InstructionEntryError>();

					err->LBL = "ERROR";
					err->FILE_LINENR = i.FILE_LINENR;
					err->descr = "Invalid label \"" + lbl + "\": "
								 + (startsReg	? "Prefixes 'R' and '$' are reserved for registers!" :
									hasSpace	? "Label cannot contain whitespaces!" :
									startsDigit ? "Label cannot start with a digit!" : "");

					this->parse_errors->push_back(err);
				}
			}

			/**	\brief	Check if a given InstructionEntry is valid based on
			 *			its mnemonic instruction name and supposed types of operands.
			 *
			 *		Things that are checked:
			 *
			 *		- Check the associated label for validity;
			 *		- If there is a label, but not a valid mnemonic Instruction name following it,
			 *			check if the name can be cast to a variable (because it would be a constant);
			 *		- If the mnemonic Instruction name does match with a known Instruction,
			 *			check if its operands are correct.
			 *			<br>(A REG is an actual string starting with CPUInstructions::REG_PREFIX,
			 *			a LBL is valid, a VAL can be cast to a value, and NONE is actually empty.)
			 *
			 *		If any of these conditions is not met, an entry will be added to the parsing error list.
			 *
			 *	\param	i
			 *		The reference to the InstructionEntry to check.
			 */
			void checkInstructionEntry(const CPUFactory::InstructionEntry &i) {
				InstructionEntryError *err = nullptr;

				if (!i.LBL.empty()) {
					this->checkInstructionEntryLabel(i, i.LBL);
				}

				if ((i.LBL.empty() || !i.NAME.empty()) && !CPUInstructions::isValidInstruction(i.NAME)) {
					if (i.LBL == "") {
						err = SysUtils::allocVar<InstructionEntryError>();
						err->LBL = "ERROR";
						err->FILE_LINENR = i.FILE_LINENR;
						err->descr = "\"" + i.NAME + "\" is not a valid instruction!";
						this->parse_errors->push_back(err);
					} else {
						bool casting_exception = false;
						std::string cast_err;

						try {
							// Will cast alpha chars to 0, so cast checking might me redundant.
							//int xx =
							SysUtils::lexical_cast<uint16_t>(i.NAME.c_str());
							//printf("Var => %s \tcast = %d\n", i.NAME.c_str(), xx);
						} catch (Exceptions::CastingException const& e) {
							casting_exception = true;
							cast_err = e.getMsg();
						}

						if (casting_exception){
							err = SysUtils::allocVar<InstructionEntryError>();
							err->LBL = "ERROR";
							err->FILE_LINENR = i.FILE_LINENR;
							err->descr = "Value expected instead of \"" + i.NAME + "\"!\n\t\t" + cast_err;
							this->parse_errors->push_back(err);
						} else {
							// Add variable to dictionary (map).
							// This will be done in SCAMAssembler (SymbolTable).
						}
					}
				} else if (!i.NAME.empty()) {
					const CPUInstructions::InstructionInfo &instr = CPUInstructions::getInstructionInfo(i.NAME);
					std::string OP;

					for (int j = 2; j; --j) {
						OP = (j == 2 ? i.OP1 : i.OP2);
						switch (j == 2 ? instr.Op1_type : instr.Op2_type) {
							case CPUInstructions::OperandType::REG: {
								bool casting_exception = false,
									 regnum_inbounds = false;
								std::string cast_err;

								try {
									int val	= SysUtils::lexical_cast<size_t>(OP.substr(1).c_str());
									regnum_inbounds = val >= 0 && val < (1 << CPUInstructions::REG_BITS_SIZE);
								} catch (Exceptions::CastingException const& e) {
									casting_exception = OP.length() > 1;
									cast_err = e.getMsg();
								}

								if (!std::strHasChar(CPUInstructions::REG_PREFIX, OP[0]) || casting_exception || !regnum_inbounds) {
									char buff[50];
									err = SysUtils::allocVar<InstructionEntryError>();
									err->LBL = "ERROR";
									err->FILE_LINENR = i.FILE_LINENR;
									err->descr = "Register expected instead of \"" + OP + "\"!"
												 + (casting_exception ? ("\n\t\t" + cast_err) : "")
												 + (!regnum_inbounds  ? SysUtils::stringFormat(buff, 50, "\n\t\tRegisters range from R0 to R%d", ((1 << CPUInstructions::REG_BITS_SIZE) - 1)) : "");
									this->parse_errors->push_back(err);
								}
								break;
							}
							case CPUInstructions::OperandType::LBL:
								this->checkInstructionEntryLabel(i, OP);
								break;
							case CPUInstructions::OperandType::VAL: {
								bool casting_exception = false;
								std::string cast_err;

								try {
									SysUtils::lexical_cast<uint16_t>(OP.c_str());
								} catch (Exceptions::CastingException const& e) {
									casting_exception = true;
									cast_err = e.getMsg();
								}

								if (casting_exception){
									err = SysUtils::allocVar<InstructionEntryError>();
									err->LBL = "ERROR";
									err->FILE_LINENR = i.FILE_LINENR;
									err->descr = "Value expected instead of \"" + OP + "\"!\n\t\t" + cast_err;
									this->parse_errors->push_back(err);
								}
								break;
							}
							case CPUInstructions::OperandType::NONE:
								if (!OP.empty()){
									err = SysUtils::allocVar<InstructionEntryError>();
									err->LBL = "ERROR";
									err->FILE_LINENR = i.FILE_LINENR;
									err->descr = "Unexpected operand \"" + i.OP1 + "\"!";
									this->parse_errors->push_back(err);
								}
								break;
							default:
								break;
						}
					}
				}
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
			SCAMParser(const std::string& filename)
				: scamFile(filename), rawFileData(nullptr), instr_entries(nullptr), parse_errors(nullptr) {
				// [SOLVED with std::strErasedToLast?]
				//		Possible unexpected behaviour if path contains '.' in dir name
				std::string ext = std::strErasedToLast(scamFile, ".");
				if (ext != SCAMParser::EXTENSION)
					throw Exceptions::UnexpectedExtension(ext);

				rawFileData = SysUtils::readStringFromFile(scamFile);
			}

			/**	\brief	Default destructor
			 */
			~SCAMParser() {
				if (this->rawFileData != nullptr)
					SysUtils::deallocVar(this->rawFileData);
				if (this->instr_entries != nullptr)
					SysUtils::deallocVector(this->instr_entries);
				if (this->parse_errors != nullptr)
					SysUtils::deallocVector(this->parse_errors);
			}

			/**	\brief	Returns the path and name of the parsed file.
			 */
			const std::string& getFileName(void) const {
				return this->scamFile;
			}

			/**	\brief	Returns the raw string data of the parsed file.
			 */
			const std::string& getRawFile(void) const {
				return *this->rawFileData;
			}

			/**	\brief	Returns whether the parsing produced any errors.
			 */
			bool hasErrors(void) {
				return this->parse_errors->size() > 0;
			}

			/**	\brief	Run the SCAMParser on this->scamFile.
			 *
			 *		Iterate over each line in the raw file, skip any comments,
			 *		and de-format the line in an InstructionEntry before calling checkInstructionEntry() on it.
			 *
			 *		This will also add two mandatory `JMP` Instructions before and after the InstructionEntry list:
			 *
			 *		- Before:
			 *		<br>	`JMP MAIN`
			 *		<br>	This will cause the program to skip constants and jump to the label called `MAIN`.
			 *		- After:
			 *		<br>	`END: JMP END`
			 *		<br>	This will cause the program to 'halt' on the last line of code (being stuck in an infinite jump cycle).
			 *
			 *	\return	Returns whether the parsing produced any errors.
			 */
			bool reparseFile(void) {
				if (this->instr_entries != nullptr) {
					SysUtils::deallocVector(this->instr_entries);
					SysUtils::deallocVector(this->parse_errors);
				}

				this->instr_entries = SysUtils::allocVar<std::vector<CPUFactory::InstructionEntry*>>();
				this->parse_errors  = SysUtils::allocVar<std::vector<CPUFactory::InstructionEntryError*>>();

				std::string line, item;
				std::stringstream ss(*this->rawFileData);
				InstructionEntry *instr = SysUtils::allocVar<InstructionEntry>();

				// Add default JMP MAIN to skip constants
				instr->FILE_LINENR	= 0;
				instr->LINENR		= 0;
				instr->NAME			= "JMP";
				instr->OP1			= "MAIN";
				this->instr_entries->push_back(instr);

				for (size_t file_line = 1; std::getline(ss, line); ++file_line) {
					std::ltrim(line);
					if (line[0] == ';' || line[0] == '/' || line.length() < 2)
						continue;	// Skip comment or empty lines

					std::strReplaceAll(line, "\t", " ");
					std::strReplaceConsecutive(line, ' ');
					std::strEraseFrom(line, ";");
					std::strEraseFrom(line, "/");
					std::rtrim(line);

					instr = SysUtils::allocVar<InstructionEntry>();
					std::stringstream iss(line);

					std::getline(iss, item, ' ');

					if (std::strHasChar(item, ':')) { // starts with label
						std::strEraseFrom(item, ":");
						instr->LBL = std::trimmed(item);
						std::getline(iss, item = "", ' ');
					} else instr->LBL = "";

					instr->NAME = std::trimmed(item);

					if (instr->NAME == instr->LBL) {
						instr->NAME = "";
					} else {
						std::getline(iss, item = "", ',');
						instr->OP1 = std::trimmed(item);

						std::getline(iss, item = "");
						instr->OP2 = std::trimmed(item);
					}

					instr->FILE_LINENR = file_line;
					instr->LINENR = this->instr_entries->size();

					//* ERROR CHECKING *//
					this->checkInstructionEntry(*instr);

					this->instr_entries->push_back(instr);
				}

				// Add default JMP END to halt execution on finish
				instr = SysUtils::allocVar<InstructionEntry>();
				instr->FILE_LINENR	= 9999;
				instr->LINENR		= this->instr_entries->size();
				instr->LBL			= "END";
				instr->NAME			= "JMP";
				instr->OP1			= "END";
				this->instr_entries->push_back(instr);

				return !this->hasErrors();
			}

			/**	\brief	Returns the parsing errors list.
			 *
			 *	**WARNING**	This will try to reparse the file if the list is not yet available.
			 */
			const std::vector<InstructionEntryError*>* getParseErrors(void) {
				if (this->parse_errors == nullptr) {
					this->reparseFile();
				}

				return this->parse_errors;
			}

			/**	\brief	Returns the InstructionEntry list.
			 *
			 *	**WARNING**	This will try to reparse the file if the list is not yet available.
			 */
			const std::vector<InstructionEntry*>* getInstructionEntries(void) {
				if (this->instr_entries == nullptr) {
					this->reparseFile();
				}

				return this->instr_entries;
			}

			/**	\brief	Returns a string with an InstructionEntry table and all parsing errors (if any).
			 */
			const std::string toString(void) {
				std::stringstream os;
				char *tmp = SysUtils::allocArray<char>(83);

				os << "SCAM-file: \"" << this->getFileName() << "\"" << std::endl;
				os << " Line | Addr |   Label    | Instr  | Operand 1  | Operand 2"   << std::endl;
				os << "------+------+------------+--------+------------+------------" << std::endl;

				for (CPUFactory::InstructionEntry* i : *this->getInstructionEntries()) {
					os << SysUtils::stringFormat(tmp, 83, " %4d | % 4d | %-10s | %-6s | %10s | %10s",
												 i->FILE_LINENR, i->LINENR, i->LBL.c_str(), i->NAME.c_str(), i->OP1.c_str(), i->OP2.c_str())
					   << std::endl;
				}

				if (this->hasErrors()) {
					os << std::endl << "SCAM-file has errors!" << std::endl;
					for (CPUFactory::InstructionEntryError* e : *this->getParseErrors()) {
						os << "Line " << SysUtils::stringFormat(tmp, 5, "% 4d", e->FILE_LINENR) << ": [" << e->LBL << "] " << e->descr
						   << std::endl;
					}
				}

				SysUtils::deallocArray(tmp);

				return os.str();
			}

			/**	\brief	Appends the output of toString() to the given stream.
			 *
			 *	\param	os
			 *		The stream to append the data to.
			 *	\param	s
			 *		A reference to the SCAMParser object to print the info from.
			 */
			friend std::ostream& operator<<(std::ostream &os, SCAMParser &s) {
				return os << s.toString();
			}

			/**	\brief	Appends the output of toString() to the given stream.
			 *
			 *	\param	os
			 *		The stream to append the data to.
			 *	\param	s
			 *		A pointer to the SCAMParser object to print the info from.
			 */
			friend std::ostream& operator<<(std::ostream &os, SCAMParser *s) {
				return os << *s;
			}

			/**	\brief	The default extenstion for a SCAMParser file: `.scam`.
			 */
			static const std::string EXTENSION;
	};

	/**	\brief	Set the default extenstion the SCAMParser to `.scam`.
	 */
	const std::string SCAMParser::EXTENSION = ".scam";
}

#endif // SCAMPARSER_HPP
