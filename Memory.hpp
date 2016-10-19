#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <array>
#include "SynchrotronComponent.hpp"
#include "Exceptions.hpp"
#include "utils.hpp"
using namespace Synchrotron;

namespace CPUComponents {

	/**	\brief
	*		TO-DO
	*/
	enum class MemoryUnits : size_t { BITS = 0, BYTES, KILOBYTES, MEGABYTES };

	/**	\brief
	*		TO-DO
	*/
	inline std::string memoryUnitToString(MemoryUnits u, bool longStr = false) {
		static std::string shortString[] = { "bits", "B", "kB", "MB" };
		static std::string longString[]  = { "bits", "Bytes", "Kilobytes", "Megabytes" };
		return (longStr ? longString : shortString)[(size_t) u];
	}

	/** \brief	**Memory** : Save the state of a SynchrotronComponent on an adress.
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the internal bitsets.
	 *	\param	mem_size
	 *		This template argument specifies the amount of internal bitsets.
	 */
	template <size_t bit_width, size_t mem_size>
	class Memory : public Mutex {
		private:
			/**
			 *	\brief	Array of std::bitset containing the memory data.
			 */
			std::bitset<bit_width> *_memory;

			/**	\brief
			 *		TO-DO
			 */
			MemoryUnits defaultUnit;

		public:
			/**
			 *	Default constructor
			 */
			Memory(MemoryUnits defaultUnit = MemoryUnits::KILOBYTES) : defaultUnit(defaultUnit) {
				#ifdef THROW_EXCEPTIONS
					if (!mem_size)
						throw Exceptions::Exception("[ERROR] Memory size is zero!");
					if (mem_size > this->getMaxSize())
						throw Exceptions::Exception("[ERROR] Memory size is to big: Insufficient adresses!");
				#endif
				LockBlock lock(this);

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
			float getSize(MemoryUnits unit) {
				float size = bit_width * mem_size;

				switch (unit) {
					case MemoryUnits::BYTES:		return size / 8;
					case MemoryUnits::KILOBYTES:	return size / 8192;
					case MemoryUnits::MEGABYTES:	return size / 8388608;
					case MemoryUnits::BITS:
					default:						return size;
				}
			}

			/**	\brief
			*		TO-DO
			*/
			size_t getMaxSize(void) {
				return (1 << bit_width) - 1;
			}

			/**	\brief
			 *		TO-DO
			 */
			MemoryUnits getSizeUnit(void) {
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
				LockBlock lock(this);
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
				LockBlock lock(this);
				this->_memory[address.to_ulong()] = data;
			}

			/**	\brief
			 *		TO-DO
			 */
			friend std::ostream& operator<<(std::ostream &os, Memory& m) {
				os << m.getSize(m.getSizeUnit()) << " " << memoryUnitToString(m.getSizeUnit() /*, true */);
				return os;
			}

			/**	\brief
			 *		TO-DO
			 */
			friend std::ostream& operator<<(std::ostream &os, Memory *m) {
				return os << *m;
			}
	};
}

#endif // MEMORY_HPP
