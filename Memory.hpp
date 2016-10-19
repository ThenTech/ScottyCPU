#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <array>
#include "SynchrotronComponent.hpp"
#include "Exceptions.hpp"
#include "utils.hpp"
using namespace Synchrotron;

namespace CPUComponents {

	/** \brief	**Memory** : Save the state of a SynchrotronComponent on an adress.
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the internal bitsets.
	 *	\param	mem_size
	 *		This template argument specifies the amount of internal bitsets.
	 */
	template <size_t bit_width, size_t mem_size>
	class Memory : public Mutex {
		public:
			/**	\brief
			 *		TO-DO
			 */
			enum class Units : size_t { BITS, BYTES, KILOBYTES, MEGABYTES };

		private:
			/**
			 *	\brief	Array of std::bitset containing the memory data.
			 */
			std::bitset<bit_width> *_memory;

			/**	\brief
			 *		TO-DO
			 */
			Memory::Units defaultUnit;

		public:
			/**
			 *	Default constructor
			 */
			Memory(Memory::Units defaultUnit = Memory::Units::KILOBYTES) : defaultUnit(defaultUnit){
				#ifdef THROW_EXCEPTIONS
					if (!mem_size)
						throw Exceptions::Exception("[ERROR] Memory size is zero!");
					if (mem_size > this->MAX_SIZE.to_ulong())
						throw Exceptions::Exception("[ERROR] Memory size is to big: Insufficient adresses!");
				#endif

				_memory = SysUtils::allocArray<std::bitset<bit_width>>(mem_size);
			}

			/**
			 *	Default destructor
			 */
			~Memory() {
				delete this->_memory;
			}

			/**	\brief
			 *		TO-DO
			 */
			float getSize(Memory::Units unit) {
				float size = bit_width * mem_size;

				switch (unit) {
					case Memory::Units::BYTES:		return size / 8;
					case Memory::Units::KILOBYTES:	return size / 8192;
					case Memory::Units::MEGABYTES:	return size / 8388608;
					case Memory::Units::BITS:
					default:						return size;
				}
			}

			/**	\brief
			 *		TO-DO
			 */
			Memory::Units getSizeUnit(void) {
				return this->defaultUnit;
			}

			/**	\brief
			 *		TO-DO
			 */
			std::bitset<bit_width>& getData(std::bitset<bit_width> address) {
				#ifdef THROW_EXCEPTIONS
					if (address.to_ulong() > mem_size)
						throw Exceptions::OutOfBoundsException("[ERROR] Memory address out of bounds!");
				#endif

				return this->_memory[address.to_ulong()];
			}

			/**	\brief
			 *		TO-DO
			 */
			void setData(std::bitset<bit_width> address, std::bitset<bit_width> data) {
				#ifdef THROW_EXCEPTIONS
					if (address.to_ulong() > mem_size)
						throw Exceptions::OutOfBoundsException("[ERROR] Memory address out of bounds!");
				#endif

				this->_memory[address.to_ulong()] = data;
			}

			/**	\brief
			 *		TO-DO
			 */
			friend std::ostream& operator<<(std::ostream &os, Memory& m) {
				os << m.getSize(m.getSizeUnit());

				switch (m.getSizeUnit()) {
					case Memory::Units::BITS:		os << " bits";	break;
					case Memory::Units::BYTES:		os << " B";		break;
					case Memory::Units::KILOBYTES:	os << " kB";	break;
					case Memory::Units::MEGABYTES:	os << " MB";	break;
					default:						os << " ?";		break;
				}

				return os;
			}

			/**	\brief
			 *		TO-DO
			 */
			friend std::ostream& operator<<(std::ostream &os, Memory *m) {
				return os << *m;
			}

			/**	\brief
			 *		TO-DO
			 */
			static std::bitset<bit_width> MAX_SIZE = std::bitset<bit_width>((1 << bit_width) - 1);
	};
}

#endif // MEMORY_HPP
