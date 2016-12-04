#ifndef SCOTTYCPU_HPP
#define SCOTTYCPU_HPP

#include "CPUComponents/ALUnit.hpp"
#include "CPUComponents/ControlUnit.hpp"
#include "CPUComponents/Memory.hpp"
#include "CPUComponents/Clock.hpp"

#include "utils.hpp"

namespace CPUComponents {

	/**
	 *	\brief	**ScottyCPU** : The class containing all components for the ScottyCPU.
	 *
	 *  \param	bit_width
	 *		This template argument specifies the width of the internal bitset state.
	 *  \param	mem_size
	 *		This template argument specifies the size of the internal memory (amount).
	 *  \param	reg_size
	 *		This template argument specifies the size of the internal registers (amount).
	 */
	template <size_t bit_width, size_t mem_size, size_t reg_size>
	class ScottyCPU {
		private:
			/**
			 *	\brief	The ALUnit for the Scotty CPU.
			 */
			ALUnit<bit_width>					*_ALU;

			/**
			 *	\brief	The ControlUnit for the Scotty CPU.
			 */
			ControlUnit<bit_width, reg_size>	*_CU;

			/**
			 *	\brief	The Memory for the Scotty CPU.
			 */
			Memory<bit_width, mem_size>			*_RAM;

			/**
			 *	\brief	The Clock for the Scotty CPU.
			 */
			Clock<bit_width>					_clk;

			/**
			 *	\brief	The common bus for the Scotty CPU.
			 */
			SynchrotronComponent<bit_width>		_BUS;

			/**
			 *	\brief	A buffer register for the ALU.
			 */
			SynchrotronComponent<bit_width>		_ALU_BUFFER;

		public:

			/** \brief	Default constructor
			 *
			 *	\param	clk_freq
			 *		The Clock frequency for the CPU.
			 */
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

			/**
			 *	\brief	Default destructor
			 */
			~ScottyCPU() {
				SysUtils::deallocVar(this->_ALU);
				SysUtils::deallocVar(this->_CU);
				SysUtils::deallocVar(this->_RAM);
			}

			/**	\brief	Returns the ALU of the CPU.
			 */
			const ALUnit<bit_width>& getALU(void) const {
				return *this->_ALU;
			}

			/**	\brief	Returns the CU of the CPU.
			 */
			const ControlUnit<bit_width, reg_size>& getContolUnit(void) const {
				return *this->_CU;
			}

			/**	\brief	Returns the Memory of the CPU.
			 */
			const Memory<bit_width, mem_size>& getRAM(void) const {
				return *this->_RAM;
			}

			/**	\brief	Returns the Clock of the CPU.
			 */
			const Clock<bit_width>& getClock(void) const {
				return this->_clk;
			}

			/**
			 *	\brief	Starts the execution of the ScottyCPU by threading the Clock.
			 */
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
