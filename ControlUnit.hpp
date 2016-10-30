#ifndef CONTROLUNIT_HPP
#define CONTROLUNIT_HPP

#include "SynchrotronComponentFixedInput.hpp"
#include "Memory.hpp"

using namespace Synchrotron;

#define	EXTRA_CPU_REGISTERS	2


namespace CPUComponents {

	template <size_t bit_width, size_t reg_size>
	class ControlUnit : public SynchrotronComponentFixedInput<bit_width, 1u> {
		private:
			Memory<bit_width, reg_size + EXTRA_CPU_REGISTERS>	*_REG;

			bitset<bit_width> REG_FLAGS_ADDR;
			bitset<bit_width> REG_ProgCounter_ADDR;

		public:

			ControlUnit()
				: _REG(SysUtils::allocVar<Memory<bit_width, reg_size + EXTRA_CPU_REGISTERS>>()),
				  REG_FLAGS_ADDR(_REG->getMaxAddress().to_ulong()),
				  REG_ProgCounter_ADDR(_REG->getMaxAddress().to_ulong() - 1)
			{
				// Empty
			}

			~ControlUnit() {
				delete this->_REG;
			}

			const Memory<bit_width, reg_size + EXTRA_CPU_REGISTERS>& getRegisters(void) const {
				return *this->_REG;
			}

			size_t getRegisterSize(void) const {
				//reg_size + EXTRA_CPU_REGISTERS
				return this->_REG->getMaxSize();
			}

			inline void tick(void) {
				std::cout << "CU ticked with clk = " << this->getInput().getState() <<std::endl;
			}
	};

}

#endif // CONTROLUNIT_HPP
