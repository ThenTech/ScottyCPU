#ifndef SCAMPARSER_HPP
#define SCAMPARSER_HPP

#include <sstream>
#include <string>
#include <vector>

#include "../CPUInstructions/Instruction.hpp"
#include "../utils.hpp"
#include "../Exceptions.hpp"

namespace CPUFactory {

	typedef struct {
		size_t		FILE_LINENR;
		size_t		LINENR;
		std::string LBL;
		std::string NAME;
		std::string OP1;
		std::string OP2;
	} InstructionEntry;

	typedef struct {
		size_t		FILE_LINENR;
		std::string LBL;
		std::string descr;
	} InstructionEntryError;


	/**
	 *	\brief	Scotty Assembler Markup
	 */
	class SCAMParser {
		protected:
			const std::string scamFile;

			const std::string *rawFileData;

			std::vector<CPUFactory::InstructionEntry*> *instr_entries;

			std::vector<CPUFactory::InstructionEntryError*> *parse_errors;

		private:
			void checkInstructionEntryLabel(const CPUFactory::InstructionEntry &i, const std::string &lbl) {
				bool startsReg   = std::strHasChar(REG_PREFIX, lbl[0]),
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
							SysUtils::lexical_cast<size_t>(i.NAME.c_str());
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
							// Add variable to dictionary (map)
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
									regnum_inbounds = val >= 0 && val < (1 << REG_BITS_SIZE);
								} catch (Exceptions::CastingException const& e) {
									casting_exception = OP.length() > 1;
									cast_err = e.getMsg();
								}

								if (!std::strHasChar(REG_PREFIX, OP[0]) || casting_exception || !regnum_inbounds) {
									char buff[50];
									err = SysUtils::allocVar<InstructionEntryError>();
									err->LBL = "ERROR";
									err->FILE_LINENR = i.FILE_LINENR;
									err->descr = "Register expected instead of \"" + OP + "\"!"
												 + (casting_exception ? ("\n\t\t" + cast_err) : "")
												 + (!regnum_inbounds  ? SysUtils::stringFormat(buff, 50, "\n\t\tRegisters range from R0 to R%d", ((1 << REG_BITS_SIZE) - 1)) : "");
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
									SysUtils::lexical_cast<size_t>(OP.c_str());
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
			SCAMParser(const std::string& filename)
				: scamFile(filename), rawFileData(nullptr), instr_entries(nullptr), parse_errors(nullptr) {
				// Possible unexpected behaviour if path contains '.' in dir name
				std::string ext = std::strErasedTo(scamFile, ".");
				if (ext != SCAMParser::EXTENSION)
					throw Exceptions::UnexpectedExtension(ext);

				rawFileData = SysUtils::readStringFromFile(scamFile);
			}

			~SCAMParser() {
				if (this->rawFileData != nullptr)
					SysUtils::deallocVar(this->rawFileData);
				if (this->instr_entries != nullptr)
					SysUtils::deallocVector(this->instr_entries);
				if (this->parse_errors != nullptr)
					SysUtils::deallocVector(this->parse_errors);
			}

			const std::string& getFileName(void) const {
				return this->scamFile;
			}

			const std::string& getRawFile(void) const {
				return *this->rawFileData;
			}

			bool hasErrors(void) {
				return this->parse_errors->size() > 0;
			}

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

			const std::vector<InstructionEntryError*>& getParseErrors(void) {
				if (this->parse_errors == nullptr) {
					this->reparseFile();
				}

				return *this->parse_errors;
			}

			const std::vector<InstructionEntry*>& getInstructionEntries(void) {
				if (this->instr_entries == nullptr) {
					this->reparseFile();
				}

				return *this->instr_entries;
			}


			const std::string toString(void) {
				std::stringstream os;
				char *tmp = SysUtils::allocArray<char>(83);

				os << "SCAM-file: \"" << this->getFileName() << "\"" << std::endl;
				os << " Line | Addr |   Label    | Instr | Operand 1  | Operand 2"   << std::endl;
				os << "------+------+------------+-------+------------+------------" << std::endl;

				for (CPUFactory::InstructionEntry* i : this->getInstructionEntries()) {
					os << SysUtils::stringFormat(tmp, 83, " %4d | % 4d | %-10s | %-5s | %10s | %10s",
												 i->FILE_LINENR, i->LINENR, i->LBL.c_str(), i->NAME.c_str(), i->OP1.c_str(), i->OP2.c_str())
					   << std::endl;
				}

				if (this->hasErrors()) {
					os << std::endl << "SCAM-file has errors!" << std::endl;
					for (CPUFactory::InstructionEntryError* e : this->getParseErrors()) {
						os << "Line " << SysUtils::stringFormat(tmp, 5, "% 4d", e->FILE_LINENR) << ": [" << e->LBL << "] " << e->descr
						   << std::endl;
					}
				}

				SysUtils::deallocArray(tmp);

				return os.str();
			}

			friend std::ostream& operator<<(std::ostream &os, SCAMParser &s) {
				return os << s.toString();
			}

			friend std::ostream& operator<<(std::ostream &os, SCAMParser *s) {
				return os << *s;
			}

			static const std::string EXTENSION;
	};

	const std::string SCAMParser::EXTENSION = ".scam";
}

#endif // SCAMPARSER_HPP
