#ifndef ALUNIT_HPP
#define ALUNIT_HPP

#include "../SynchrotronComponentFixedInput.hpp"
#include "../CPUInstructions/Instruction.hpp"
#include "../CPUInstructions/ANDInstruction.hpp"
#include "../CPUInstructions/NANDInstruction.hpp"
#include "../CPUInstructions/ORInstruction.hpp"
#include "../CPUInstructions/NORInstruction.hpp"
#include "../CPUInstructions/XORInstruction.hpp"
#include "../CPUInstructions/NOTInstruction.hpp"
#include "../CPUInstructions/SHLInstruction.hpp"
#include "../CPUInstructions/SHRInstruction.hpp"
#include "../CPUInstructions/ADDInstruction.hpp"
#include "../CPUInstructions/SUBInstruction.hpp"
#include "../CPUInstructions/MULInstruction.hpp"
#include "../CPUInstructions/DIVInstruction.hpp"
#include "../CPUInstructions/MODInstruction.hpp"
#include "../CPUInstructions/CMPInstruction.hpp"

using namespace Synchrotron;
using namespace CPUInstructions;

namespace CPUComponents {

	/** \brief	**ALUnit** : The Arithmic and Logic Unit for the CPU.
	 *
	 *			Contains the other CPUComponents linked to its inputs to execute instructions.
	 *
	 *			Can only have 2 inputs: 2 registers to perform calcuations.
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class ALUnit : public SynchrotronComponentFixedInput<bit_width, 2u>, public FlagRegister {
		private:
			/**	\brief	The ALU instruction to execute.
			 */
			InstructionSet operation;

			/**	\brief	ALU Instructions AND
			 */
			ANDInstruction<bit_width>	_AND;

			/**	\brief	ALU Instructions NAND
			 */
			NANDInstruction<bit_width>	_NAND;

			/**	\brief	ALU Instructions OR
			 */
			ORInstruction<bit_width>	_OR;

			/**	\brief	ALU Instructions NOR
			 */
			NORInstruction<bit_width>	_NOR;

			/**	\brief	ALU Instructions XOR
			 */
			XORInstruction<bit_width>	_XOR;

			/**	\brief	ALU Instructions NOT
			 */
			NOTInstruction<bit_width>	_NOT;

			/**	\brief	ALU Instructions ADD
			 */
			ADDInstruction<bit_width>	_ADD;

			/**	\brief	ALU Instructions SUB
			 */
			SUBInstruction<bit_width>	_SUB;

			/**	\brief	ALU Instructions MUL
			 */
			MULInstruction<bit_width>	_MUL;

			/**	\brief	ALU Instructions DIV
			 */
			DIVInstruction<bit_width>	_DIV;

			/**	\brief	ALU Instructions MOD
			 */
			MODInstruction<bit_width>	_MOD;

			/**	\brief	ALU Instructions SHL
			 */
			SHLInstruction<bit_width>	_SHL;

			/**	\brief	ALU Instructions SHR
			 */
			SHRInstruction<bit_width>	_SHR;

			/**	\brief	ALU Instructions CMP
			 */
			CMPInstruction<bit_width>	_CMP;

		public:

			/** \brief	Default constructor
			 */
			ALUnit() : operation(InstructionSet::NOP) {}

			/** \brief	Default destructor
			 */
			~ALUnit() {}

			/**	\brief
			 *		Connect the internal instructions with the ALU's inputs.
			 *
			 *		ALU should have 2 inputs connected.
			 */
			void connectInternal(void) {
				if (this->getInputs().size() < 2)
					//throw Exceptions::Exception("[ERROR] ALUnit connectInternal requires tha ALU to have exactly 2 inputs!");
					return;

				std::initializer_list<SynchrotronComponent<bit_width>*> inputList
						= { *this->getInputs().begin(), *this->getInputs().rbegin() };

				this->_AND.addInput(inputList);
				this->_NAND.addInput(inputList);
				this->_OR.addInput(inputList);
				this->_NOR.addInput(inputList);
				this->_XOR.addInput(inputList);
				this->_NOT.addInput(**this->getInputs().begin());
				this->_ADD.addInput(inputList);
				this->_SUB.addInput(inputList);
				this->_MUL.addInput(inputList);
				this->_DIV.addInput(inputList);
				this->_MOD.addInput(inputList);
				this->_SHL.addInput(**this->getInputs().begin());
				this->_SHR.addInput(**this->getInputs().begin());
				this->_CMP.addInput(inputList);

				this->removeInput(**this->getInputs().begin());
				this->removeInput(**this->getInputs().begin());
			}

			/**	\brief	Set the operation/instruction to execute on tick().
			 *
			 *	\param	_instr
			 *		The Instruction to set as the current one to execute.
			 */
			void setOperation(InstructionSet _instr) {
				this->operation = _instr;
			}

			void clearFlagsReg(void) {
				this->clearFlags();
			}

			/**	\brief	The tick() method will be called when one of this SynchrotronComponent's inputs issues an emit().
			 *			Get the state and flags from the expected Instruction.
			 *
			 *	\return	virtual void
			 *		This method should be implemented by a derived class.
			 */
			void tick(void) {
				std::bitset<bit_width> prevState = this->state;

				switch(this->operation) {
					// TO-DO : Replace "this->REG_FLAGS	= this->_AND.getFlags();"
					//				by "this->REG_FLAGS	= this->_AND;"
					//				or "this->REG_FLAGS	= (FlagRegister) this->_AND;"
					//				instead?
					case InstructionSet::AND:	this->state = this->_AND.getState();	this->setFlags(this->_AND.getFlags());	break;
					case InstructionSet::NAND:	this->state = this->_NAND.getState();	this->setFlags(this->_NAND.getFlags());	break;
					case InstructionSet::OR:	this->state = this->_OR.getState();		this->setFlags(this->_OR.getFlags()); 	break;
					case InstructionSet::NOR:	this->state = this->_NOR.getState();	this->setFlags(this->_NOR.getFlags()); 	break;
					case InstructionSet::XOR:	this->state = this->_XOR.getState();	this->setFlags(this->_XOR.getFlags());	break;
					case InstructionSet::NOT:	this->state = this->_NOT.getState();	this->setFlags(this->_NOT.getFlags()); 	break;
					case InstructionSet::ADD:	this->state = this->_ADD.getState();	this->setFlags(this->_ADD.getFlags());	break;
					case InstructionSet::SUB:	this->state = this->_SUB.getState();	this->setFlags(this->_SUB.getFlags());	break;
					case InstructionSet::MUL:	this->state = this->_MUL.getState();	this->setFlags(this->_MUL.getFlags());	break;
					case InstructionSet::DIV:	this->state = this->_DIV.getState();	this->setFlags(this->_DIV.getFlags());	break;
					case InstructionSet::MOD:	this->state = this->_MOD.getState();	this->setFlags(this->_MOD.getFlags());	break;
					case InstructionSet::SHL:	this->state = this->_SHL.getState();	this->setFlags(this->_SHL.getFlags());	break;
					case InstructionSet::SHR:	this->state = this->_SHR.getState();	this->setFlags(this->_SHR.getFlags());	break;
					case InstructionSet::CMP:	this->state = this->_CMP.getState();	this->setFlags(this->_CMP.getFlags());	break;
					case InstructionSet::NOP:	break;
					default:
						throw Exceptions::Exception("ALU: Unsupported Arithmetic or Logic operation!");
						break;
				}

				if (prevState != this->state) this->emit();
			}
	};

}


#endif // ALUNIT_HPP
