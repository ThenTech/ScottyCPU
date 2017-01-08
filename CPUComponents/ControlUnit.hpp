#ifndef CONTROLUNIT_HPP
#define CONTROLUNIT_HPP

#include "../SynchrotronComponentFixedInput.hpp"
#include "../CPUInstructions/Instruction.hpp"
#include "ALUnit.hpp"
#include "Memory.hpp"
#include "MemoryCell.hpp"

using namespace Synchrotron;

/** \brief	Extra registers for the CU.
 */
#define	EXTRA_CPU_REGISTERS	3


namespace CPUComponents {

	/** \brief	**ControlUnit** : The Control Unit for the CPU.
	 *
	 *			Parses and executes instructions. Contains temporary registers.
	 *
	 *			Can only have 1 input: the Clock.
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the internal bitset state.
	 */
	template <size_t bit_width, size_t mem_size, size_t reg_size>
	class ControlUnit : public SynchrotronComponentFixedInput<bit_width, 1u> {
		private:
			/**	\brief	Temporary registers for calculations.
			 */
			Memory<bit_width, reg_size + EXTRA_CPU_REGISTERS>	*_REG;

			/**	\brief	Address of flags register.
			 */
			const std::bitset<bit_width> REG_FLAGS_ADDR;

			/**	\brief	Address of register containing the address in RAM to execute.
			 */
			const std::bitset<bit_width> REG_PROGRAM_COUNTER_ADDR;

			/**	\brief	Address of register containing the instruction to execute.
			 */
			const std::bitset<bit_width> REG_INTRUCTION_REGISTER_ADDR;

			ALUnit<bit_width>			*_ALU;
			Memory<bit_width, mem_size>	*_RAM;
			MemoryCell<bit_width>		*_BUS;
			MemoryCell<bit_width>		*_ALU_BUFFER;

			void fetchNextInstruction(void) {
				// Fetch instruction from _RAM[ _REG[REG_PROGRAM_COUNTER_ADDR] ] and increment ProgramCounter
				this->_REG->setData(REG_INTRUCTION_REGISTER_ADDR,
									this->_RAM->getData(this->_REG->getData(REG_PROGRAM_COUNTER_ADDR)));

				this->_ALU->setOperation(InstructionSet::ADD);
				this->_BUS->setState(this->_REG->getData(REG_PROGRAM_COUNTER_ADDR));
				this->_ALU_BUFFER->setState(std::bitset<bit_width>(1u));
				this->_BUS->tick();
				this->_ALU->tick();

				this->_REG->setData(REG_PROGRAM_COUNTER_ADDR, this->_ALU->getState());

				std::cout << "After:\n";
				std::cout << "REG_PROGRAM_COUNTER: " << this->_REG->getData(REG_PROGRAM_COUNTER_ADDR).to_string() << std::endl;
				std::cout << "REG_INTRUCTION_REG : " << this->_REG->getData(REG_INTRUCTION_REGISTER_ADDR).to_string() << std::endl;
			}

			void setNextInstrAsPC(void) {
				this->fetchNextInstruction();
				this->_REG->setData(REG_PROGRAM_COUNTER_ADDR,
									this->_REG->getData(REG_INTRUCTION_REGISTER_ADDR));
			}

		public:

			/** \brief	Default constructor
			 */
			ControlUnit(ALUnit<bit_width>			*ALU,
						Memory<bit_width, mem_size>	*RAM,
						MemoryCell<bit_width>		*BUS,
						MemoryCell<bit_width>		*ALU_BUFFER)
				: _REG(SysUtils::allocVar<Memory<bit_width, reg_size + EXTRA_CPU_REGISTERS>>()),
				  REG_FLAGS_ADDR(_REG->getMaxAddress().to_ulong()),
				  REG_PROGRAM_COUNTER_ADDR(_REG->getMaxAddress().to_ulong() - 1),
				  REG_INTRUCTION_REGISTER_ADDR(_REG->getMaxAddress().to_ulong() - 2),
				  _ALU(ALU), _RAM(RAM), _BUS(BUS), _ALU_BUFFER(ALU_BUFFER)
			{
				#ifdef THROW_EXCEPTIONS
					if (this->_ALU == nullptr)
						throw Exceptions::Exception("ControlUnit: No ALU connected!");
					if (this->_RAM == nullptr)
						throw Exceptions::Exception("ControlUnit: No RAM connected!");
					if (this->_BUS == nullptr)
						throw Exceptions::Exception("ControlUnit: No BUS connected!");
					if (this->_ALU_BUFFER == nullptr)
						throw Exceptions::Exception("ControlUnit: No ALU_BUFFER connected!");
				#endif
			}

			/** \brief	Default destructor
			 */
			~ControlUnit() {
				SysUtils::deallocVar(this->_REG);
			}

			/**
			 *	\brief	Gets the CU's registers
			 *
			 *	\return	Memory<bit_width, reg_size + EXTRA_CPU_REGISTERS>
			 *      Returns the CU's registers.
			 */
			/* const */ Memory<bit_width, reg_size + EXTRA_CPU_REGISTERS>& getRegisters(void) const {
				return *this->_REG;
			}

			const std::bitset<bit_width>& getFlagReg(void) const {
				return this->_REG->getData(this->REG_FLAGS_ADDR);
			}

			const std::bitset<bit_width>& getProgramCouterReg(void) const {
				return this->_REG->getData(this->REG_PROGRAM_COUNTER_ADDR);
			}

			const std::bitset<bit_width>& getInstructionReg(void) const {
				return this->_REG->getData(this->REG_INTRUCTION_REGISTER_ADDR);
			}

			/**	\brief	Gets amount of CU Registers.
			 *
			 *	\return	size_t
			 *      Returns size of the registers.
			 */
			size_t getRegisterSize(void) const {
				//reg_size + EXTRA_CPU_REGISTERS
				return this->_REG->getMaxAddress().to_ulong();
			}

			/**	\brief	The tick() method will be called when the connected clock ticks.
			 */
			void tick(void) {
				std::cout << "CU ticked with clk = " << this->getInput().getState() << std::endl;

				this->fetchNextInstruction();

				const uint8_t instr = (this->_REG->getData(REG_INTRUCTION_REGISTER_ADDR) >> 8).to_ulong() & 0xFF;
				const uint8_t idata = this->_REG->getData(REG_INTRUCTION_REGISTER_ADDR).to_ulong() & 0xFF;
				const std::bitset<bit_width> REG_A_ADDR(idata >> 4);
				const std::bitset<bit_width> REG_B_ADDR(idata & 0xF);

				if (instr > 0 && instr <= UINT(InstructionSet::CMP)) {
					// ALU instruction
					this->_ALU->setOperation((InstructionSet) instr);
					this->_BUS->setState(this->_REG->getData(REG_A_ADDR));
					this->_ALU_BUFFER->setState(this->_REG->getData(REG_B_ADDR));
					this->_BUS->tick();
					this->_ALU->tick();

					if (instr < UINT(InstructionSet::CMP)) { // instr != CMP, because CMP does not set the output state
						this->_REG->setData(REG_A_ADDR, this->_ALU->getState());
					}

					this->_REG->setData(REG_FLAGS_ADDR,
										std::bitset<bit_width>(this->_ALU->getFlags().to_ulong()));
				} else {
					this->_ALU->setOperation(InstructionSet::NOP);

					switch((InstructionSet) instr) {
						// Handled in previous block
	//					case InstructionSet::ADD: break;
	//					case InstructionSet::SUB: break;
	//					case InstructionSet::MUL: break;
	//					case InstructionSet::DIV: break;
	//					case InstructionSet::MOD: break;
	//					case InstructionSet::SHL: break;
	//					case InstructionSet::SHR: break;
	//					//case InstructionSet::ROR: break;
	//					//case InstructionSet::ROL: break;
	//					case InstructionSet::CMP: break;
	//					case InstructionSet::NOT: break;
	//					case InstructionSet::AND: break;
	//					case InstructionSet::NAND: break;
	//					case InstructionSet::OR: break;
	//					case InstructionSet::NOR: break;
	//					case InstructionSet::XOR: break;

						case InstructionSet::MOV: //InstructionSet::MOVRR:
							// Copy R_A <- R_B
							this->_REG->setData(REG_A_ADDR,
												this->_REG->getData(REG_B_ADDR));
							break;
						case InstructionSet::MOVRA:
							// Copy R_A <- _RAM[idata]
							this->fetchNextInstruction();
							this->_REG->setData(REG_A_ADDR,
												this->_RAM->getData(this->_REG->getData(REG_INTRUCTION_REGISTER_ADDR)));
							break;
						case InstructionSet::MOVAR:
							// Copy _RAM[idata] <- R_B
							this->fetchNextInstruction();
							this->_RAM->setData(this->_REG->getData(REG_INTRUCTION_REGISTER_ADDR),
												this->_REG->getData(REG_B_ADDR));
							break;
						case InstructionSet::MOVAA:
							{
							// Copy _RAM[idata] <- _RAM[next_instr]
							this->fetchNextInstruction();
							std::bitset<bit_width> tmpdata = this->_REG->getData(REG_INTRUCTION_REGISTER_ADDR);

							this->fetchNextInstruction();
							this->_RAM->setData(tmpdata,
												this->_RAM->getData(this->_REG->getData(REG_INTRUCTION_REGISTER_ADDR)));
							}
							break;
						case InstructionSet::DATA:
						case InstructionSet::DATAC:
							this->fetchNextInstruction();
							this->_REG->setData(REG_A_ADDR,
												this->_REG->getData(REG_INTRUCTION_REGISTER_ADDR));
							break;
						case InstructionSet::NOP:
							// Nothing
							break;
						case InstructionSet::JMP:
							this->setNextInstrAsPC();
							break;
						case InstructionSet::JE:
							if (this->_ALU->flagIsSet(CPUInstructions::FLAGS::Equal)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JL:
							if (this->_ALU->flagIsSet(CPUInstructions::FLAGS::Larger)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JS:
							if (this->_ALU->flagIsSet(CPUInstructions::FLAGS::Smaller)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JZ:
							if (this->_ALU->flagIsSet(CPUInstructions::FLAGS::Zero)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JLE:
							if (	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Larger)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Equal)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JSE:
							if (	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Smaller)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Equal)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JCE:
							if (	this->_ALU->flagIsSet(CPUInstructions::FLAGS::CarryOut)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Equal)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JCL:
							if (	this->_ALU->flagIsSet(CPUInstructions::FLAGS::CarryOut)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Larger)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JCS:
							if (	this->_ALU->flagIsSet(CPUInstructions::FLAGS::CarryOut)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Smaller)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JCZ:
							if (	this->_ALU->flagIsSet(CPUInstructions::FLAGS::CarryOut)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Zero)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JLZ:
							if (	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Larger)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Zero)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JSZ:
							if (	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Smaller)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Zero)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JEZ:
							if (	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Equal)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Zero)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JCLE:
							if (	this->_ALU->flagIsSet(CPUInstructions::FLAGS::CarryOut)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Larger)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Equal)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JCLZ:
							if (	this->_ALU->flagIsSet(CPUInstructions::FLAGS::CarryOut)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Larger)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Zero)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JCEZ:
							if (	this->_ALU->flagIsSet(CPUInstructions::FLAGS::CarryOut)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Equal)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Zero)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JLEZ:
							if (	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Larger)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Equal)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Zero)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::JCLEZ:
							if (	this->_ALU->flagIsSet(CPUInstructions::FLAGS::CarryOut)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Larger)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Equal)
								||	this->_ALU->flagIsSet(CPUInstructions::FLAGS::Zero)) {
								this->setNextInstrAsPC();
							}
							break;
						case InstructionSet::CLF:
							this->_ALU->clearFlagsReg();
							this->_REG->resetData(REG_FLAGS_ADDR);
							break;
						default:
							break;
					}
				}
			}
	};

}

#endif // CONTROLUNIT_HPP
