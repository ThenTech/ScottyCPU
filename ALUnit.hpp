#ifndef ALUNIT_HPP
#define ALUNIT_HPP

#include "SynchrotronComponentFixedInput.hpp"
#include "ANDGate.hpp"
#include "NANDGate.hpp"
#include "ORGate.hpp"
#include "NORGate.hpp"
#include "XORGate.hpp"
#include "NOTGate.hpp"
#include "SHIFTLeft.hpp"
#include "SHIFTRight.hpp"
#include "Instructions/Instruction.hpp"

using namespace Synchrotron;
using namespace CPUInstructions;

namespace CPUComponents {

	template <size_t bit_width>
	class ALUnit : public SynchrotronComponentFixedInput<bit_width, 2u> {
		private:

			InstructionSet operation;

			bitset<bit_width> REG_FLAGS;

			ANDGate<bit_width>		_AND;
			NANDGate<bit_width>		_NAND;
			ORGate<bit_width>		_OR;
			NORGate<bit_width>		_NOR;
			XORGate<bit_width>		_XOR;
			NOTGate<bit_width>		_NOT;
			SHIFTLeft<bit_width>	_SHL;
			SHIFTRight<bit_width>	_SHR;

			SynchrotronComponent<bit_width>	result;

		public:

			ALUnit() : operation(InstructionSet::NOP) {

			}

			~ALUnit() {
			}

			void connectInternal(void) {
				std::initializer_list<SynchrotronComponent<bit_width>*> inputList = { *this->getInputs().begin(), *this->getInputs().end() };

				this->_AND.addInput(inputList);
				this->_NAND.addInput(inputList);
				this->_OR.addInput(inputList);
				this->_NOR.addInput(inputList);
				this->_XOR.addInput(inputList);
				this->_NOT.addInput(inputList);
				this->_SHL.addInput(inputList);
				this->_SHR.addInput(inputList);
			}

			void setOperation(InstructionSet instr) {
				this->operation = instr;
			}

			inline void tick(void) {
				std::bitset<bit_width> prevState = this->state;

				switch(this->operation) {
					case InstructionSet::AND:	this->state = this->_AND.getState();	break;
					case InstructionSet::NAND:	this->state = this->_NAND.getState();	break;
					case InstructionSet::OR:	this->state = this->_OR.getState();		break;
					case InstructionSet::NOR:	this->state = this->_NOR.getState();	break;
					case InstructionSet::XOR:	this->state = this->_XOR.getState();	break;
					case InstructionSet::NOT:	this->state = this->_NOT.getState();	break;
					case InstructionSet::SHL:	this->state = this->_SHL.getState();	break;
					case InstructionSet::SHR:	this->state = this->_SHR.getState();	break;
					default:
						throw Exceptions::Exception("ALU: Unsupported Arithmetic or Logic operation!");
						break;
				}

				if (prevState != this->state) this->emit();
			}
	};

}


#endif // ALUNIT_HPP
