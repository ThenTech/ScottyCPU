#ifndef SCAMPARSER_HPP
#define SCAMPARSER_HPP

#include <sstream>
#include <string>
#include <vector>

#include "../utils.hpp"
#include "../Exceptions.hpp"

namespace CPUFactory {

	typedef struct {
		size_t		LINENR;
		std::string LBL;
		std::string NAME;
		std::string OP1;
		std::string OP2;
	} InstructionEntry;

	/**
	 *	\brief	Scotty Assembler Markup
	 */
	class SCAMParser {
		protected:
			const std::string scamFile;

			const std::string *rawFileData;

			std::vector<CPUFactory::InstructionEntry*> *instr_entries;

		public:
			SCAMParser(const std::string& filename) : scamFile(filename), rawFileData(nullptr), instr_entries(nullptr) {
				std::string ext = std::strErasedTo(scamFile, ".");
				if (ext != SCAMParser::EXTENSION)
					throw Exceptions::UnexpectedExtension(ext);

				rawFileData = SysUtils::readStringFromFile(scamFile);
			}

			~SCAMParser() {
				SysUtils::deallocVar(this->rawFileData);

				for (CPUFactory::InstructionEntry* i : *this->instr_entries) {
					SysUtils::deallocVar(i);
				}

				SysUtils::deallocVar(this->instr_entries);
			}

			const std::string& getFileName(void) const {
				return this->scamFile;
			}

			const std::string& getRawFile(void) const {
				return *this->rawFileData;
			}

			const std::vector<InstructionEntry*>& getInstructionEntries(void) {
				if (this->instr_entries == nullptr) {
					this->instr_entries = SysUtils::allocVar<std::vector<CPUFactory::InstructionEntry*>>();

					std::string line, item;
					std::stringstream ss(*this->rawFileData);

					while (std::getline(ss, line)) {
						std::ltrim(line);
						if (line[0] == ';' || line[0] == '/' || line.length() < 2)
							continue;	// Skip comment or empty lines

						std::strReplaceAll(line, "\t", " ");
						std::strEraseFrom(line, ";");
						std::strEraseFrom(line, "/");
						std::rtrim(line);

						InstructionEntry *instr = SysUtils::allocVar<InstructionEntry>();
						std::stringstream iss(line);

						std::getline(iss, item, ' ');

						if (item.find(':') != std::string::npos) { // starts with label
							std::strEraseFrom(item, ":");
							instr->LBL = std::trimmed(item);
							std::getline(iss, item, ' ');
						} else instr->LBL = "";

						instr->NAME = std::trimmed(item);

						if (instr->NAME == instr->LBL) {
							instr->NAME = "";
						} else {
							std::getline(iss, item, ',');
							instr->OP1 = std::trimmed(item);

							std::getline(iss, item);
							instr->OP2 = std::trimmed(item);
						}

						instr->LINENR = this->instr_entries->size();

						this->instr_entries->push_back(instr);
					}
				}

				return *this->instr_entries;
			}

			friend std::ostream& operator<<(std::ostream &os, SCAMParser &s) {
				char *tmp = SysUtils::allocArray<char>(83);

				os << "SCAM-file: \"" << s.getFileName() << "\"" << std::endl;
				os << " Line |   Label    | Instr | Operand 1  | Operand 2" << std::endl;
				os << "------+------------+-------+------------+------------" << std::endl;

				for (CPUFactory::InstructionEntry* i : s.getInstructionEntries()) {
					os << SysUtils::stringFormat(tmp, 83, " % 4d | %-10s | %-5s | %10s | %10s",
												 i->LINENR, i->LBL.c_str(), i->NAME.c_str(), i->OP1.c_str(), i->OP2.c_str())
					   << std::endl;
				}

				SysUtils::deallocArray(tmp);
				return os;
			}

			friend std::ostream& operator<<(std::ostream &os, SCAMParser *s) {
				return os << *s;
			}

			static const std::string EXTENSION;
	};

	const std::string SCAMParser::EXTENSION = ".scam";
}

#endif // SCAMPARSER_HPP
