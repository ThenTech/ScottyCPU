#ifndef SCOTTYCPU_HPP
#define SCOTTYCPU_HPP

#include <ratio>

#include "ALUnit.hpp"
#include "ControlUnit.hpp"
#include "Memory.hpp"
#include "Clock.hpp"

#include "utils.hpp"

namespace CPUComponents {

	/**
	 *	\brief	TO-DO
	 *
	 *  \param	bit_width
	 *		The bit width.
	 *  \param	mem_siz
	 *		The memory size.
	 *  \param	reg_siz
	 *		The registry size.
	 */
	template <size_t bit_width, size_t mem_size, size_t reg_size>
	class ScottyCPU {
		private:

			ALUnit<bit_width>					*_ALU;

			ControlUnit<bit_width, reg_size>	*_CU;

			Memory<bit_width, mem_size>			*_RAM;

			Clock<bit_width>					_clk;

			SynchrotronComponent<bit_width>		_BUS;
			SynchrotronComponent<bit_width>		_ALU_BUFFER;

		public:

			ScottyCPU(float clk_freq)
				: _ALU(SysUtils::allocVar<ALUnit<bit_width>>()),
				  _CU(SysUtils::allocVar<ControlUnit<bit_width, reg_size>>()),
				  _RAM(SysUtils::allocVar<Memory<bit_width, mem_size>>()),
				  _clk(clk_freq)
			{
				this->_clk.addOutput(*this->_CU);
				this->_ALU->addInput(this->_BUS);
				this->_ALU->addInput(this->_ALU_BUFFER);
			}

			~ScottyCPU() {
				delete this->_CU;
				delete this->_RAM;
			}

			const ALUnit<bit_width>& getALU(void) const {
				return *this->_ALU;
			}

			const ControlUnit<bit_width, reg_size>& getContolUnit(void) const {
				return *this->_CU;
			}

			const Memory<bit_width, mem_size>& getRAM(void) const {
				return *this->_RAM;
			}

			const Clock<bit_width>& getClock(void) const {
				return this->_clk;
			}

			void start(void) {
				// Thread cannot access bitset output?

				//std::thread clk(_clk);
				//clk.join();

				this->_clk();

				//_clk.startThread();
			}
	};

}


#endif // SCOTTYCPU_HPP
