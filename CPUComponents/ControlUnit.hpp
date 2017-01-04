#ifndef CONTROLUNIT_HPP
#define CONTROLUNIT_HPP

#include "../SynchrotronComponentFixedInput.hpp"
#include "Memory.hpp"

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
	template <size_t bit_width, size_t reg_size>
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

		public:

			/** \brief	Default constructor
			 */
			ControlUnit()
				: _REG(SysUtils::allocVar<Memory<bit_width, reg_size + EXTRA_CPU_REGISTERS>>()),
				  REG_FLAGS_ADDR(_REG->getMaxAddress().to_ulong()),
				  REG_PROGRAM_COUNTER_ADDR(_REG->getMaxAddress().to_ulong() - 1),
				  REG_INTRUCTION_REGISTER_ADDR(_REG->getMaxAddress().to_ulong() - 2)
			{
				// Empty
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
			const Memory<bit_width, reg_size + EXTRA_CPU_REGISTERS>& getRegisters(void) const {
				return *this->_REG;
			}

			/**	\brief	Gets amount of CU Registers.
			 *
			 *	\return	size_t
			 *      Returns size of the registers.
			 */
			size_t getRegisterSize(void) const {
				//reg_size + EXTRA_CPU_REGISTERS
				return this->_REG->getMaxSize();
			}

			/**	\brief	The tick() method will be called when the connected clock ticks.
			 */
			void tick(void) {
				std::cout << "CU ticked with clk = " << this->getInput().getState() <<std::endl;
			}
	};

}

#endif // CONTROLUNIT_HPP
