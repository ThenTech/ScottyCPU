#ifndef SCOTTYCPU_HPP
#define SCOTTYCPU_HPP

#include "CPUComponents/ALUnit.hpp"
#include "CPUComponents/ControlUnit.hpp"
#include "CPUComponents/Memory.hpp"
#include "CPUComponents/Clock.hpp"
#include "CPUComponents/MemoryCell.hpp"

#include "utils.hpp"

namespace CPUComponents {

	/**
	 *	\brief	**ScottyCPU** : The class containing all components for the ScottyCPU.
	 *
	 *		* Currently limited to bit_width of 16 (8, 16, 24, 32 ... should be possible).
	 *		* Currently only support staticly loading 1 program from address 0 in RAM.
	 *
	 *  \tparam	bit_width
	 *		This template argument specifies the width of the internal bitset state.
	 *  \tparam	mem_size
	 *		This template argument specifies the size of the internal memory (amount).
	 *  \tparam	reg_size
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
			 *	\brief	The Memory for the Scotty CPU.
			 */
			Memory<bit_width, mem_size>			*_RAM;

			/**
			 *	\brief	The ControlUnit for the Scotty CPU.
			 */
			ControlUnit<bit_width, mem_size, reg_size>	*_CU;

			/**
			 *	\brief	The Clock for the Scotty CPU.
			 */
			Clock<bit_width>					_clk;

			/**
			 *	\brief	The common bus for the Scotty CPU.
			 */
			MemoryCell<bit_width>				_BUS;

			/**
			 *	\brief	A buffer register for the ALU.
			 */
			MemoryCell<bit_width>				_ALU_BUFFER;

		public:

			/** \brief	Default constructor
			 *
			 *	\param	clk_freq
			 *		The Clock frequency for the CPU.
			 *	\exception	Exceptions::Exception
			 *		Throws Exception if the bit_width is not 16-bits.
			 *		Currently the ScottyCPU is fixed to a 16-bit architecure.
			 */
			ScottyCPU(float clk_freq)
				: _ALU(SysUtils::allocVar<ALUnit<bit_width>>()),
				  _RAM(SysUtils::allocVar<Memory<bit_width, mem_size>>()),
				 // _CU(SysUtils::allocVar<ControlUnit<bit_width, mem_size, reg_size>(_ALU, _RAM, &_BUS, &_ALU_BUFFER)>()),
				  _CU(new ControlUnit<bit_width, mem_size, reg_size>(_ALU, _RAM, &_BUS, &_ALU_BUFFER)),
				  _clk(clk_freq)
			{
				#ifdef THROW_EXCEPTIONS
					if (bit_width != 16u)
						throw Exceptions::Exception("ScottyCPU is currently locked with a bit_width of 16!");
				#endif
				this->_clk.addOutput(*this->_CU);
				this->_ALU->addInput(this->_BUS);
				this->_ALU->addInput(this->_ALU_BUFFER);
				this->_ALU->connectInternal();
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
			const ALUnit<bit_width>* getALU(void) const {
				return this->_ALU;
			}

			/**	\brief	Returns the CU of the CPU.
			 */
			const ControlUnit<bit_width, mem_size, reg_size>* getControlUnit(void) const {
				return this->_CU;
			}

			/**	\brief	Returns the Memory of the CPU.
			 */
			Memory<bit_width, mem_size>& getRAM(void) const {
				return *this->_RAM;
			}

			/**	\brief	Returns the Clock of the CPU.
			 */
			Clock<bit_width>& getClock(void) /* const */ {
				return this->_clk;
			}

			/**	\brief	Staticly loads a program into RAM (from address 0).
			 *
			 *	\param	*buffer
			 *			A vector with buffer->size() bytes containing a program in raw binary.
			 *
			 *	\throws	Exceptions::OutOfBoundsException
			 *			Throws Exception if program is longer than the available Memory.
			 */
			void staticLoader(const std::vector<char> *buffer) {
				// split into bit_width segments when pushing into RAM
				try {
					std::bitset<bit_width> data;
					size_t bytes_per_instruction = bit_width / 8;
					int i;

					for (size_t pos = 0, address = 0; pos < buffer->size(); ++address) {
						data = 0;
						i = bytes_per_instruction;

						while(i--) {
							data <<= 8;
							data |= 0xFF & buffer->at(pos++);
						}

						this->_RAM->setData(std::bitset<bit_width>(address), data);
					}
				} catch (Exceptions::OutOfBoundsException const& e) {
					throw Exceptions::Exception(e.getMessage() + "\n[ScottyCPU::staticLoader] : \n\tProgram to be loaded is longer than the available Memory!");
				}
			}


			/**
			 *	\brief	Starts the execution of the ScottyCPU by threading the Clock.
			 */
			void start(void) {
				// Thread cannot access bitset output?

				//std::thread clk(_clk);
				//clk.join();

				this->_clk();

				//this->_clk.startThread();
			}

			/**
			 *	\brief	Steps the execution of the ScottyCPU by one tick().
			 *			Also dumps the contents of its RAM and Registers respectively
			 *			to RAMdump.txt and REGdump.txt.
			 */
			void step(void) {
				std::bitset<bit_width> *range = this->getRAM().getDataRange(0, this->getRAM().getMaxAddress());
				std::stringstream ss;
				for (size_t i = 0; i < this->getRAM().getMaxAddress().to_ulong(); ++i)
					ss << range[i].to_string() << std::endl;

				SysUtils::writeStringToFile("RAMdump.txt", ss.str());

				SysUtils::deallocArray(range);
				range = nullptr;


				range = this->getControlUnit()->getRegisters().getDataRange(0, this->getControlUnit()->getRegisters().getMaxAddress());
				std::stringstream st;
				for (size_t i = 0; i < this->getControlUnit()->getRegisterSize(); ++i)
					st << range[i].to_string() << std::endl;

				SysUtils::writeStringToFile("REGdump.txt", st.str());

				SysUtils::deallocArray(range);


				this->getClock().tick();
			}
	};

}


#endif // SCOTTYCPU_HPP
