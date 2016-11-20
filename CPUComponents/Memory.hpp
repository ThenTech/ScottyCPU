#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "SynchrotronComponent.hpp"
#include "Exceptions.hpp"
#include "utils.hpp"
using namespace Synchrotron;

namespace CPUComponents {

	/**	\brief	Enum containing different units to express memory sizes.
	 */
	enum class MemoryUnits : size_t { BITS = 0, BYTES, KILOBYTES, MEGABYTES };

	/**	\brief	Return the string representation for the memory units.
	 *
	 *	\param	u
	 *		The given memory unit.
	 *	\param	longStr
	 *		Specifies whether to return a shortend or the full name of the unit.
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
			 *	\brief	Array of `std::bitset` containing the memory data.
			 */
			std::bitset<bit_width> *_memory;

			/**	\brief	The default unit to display data in.
			 */
			MemoryUnits defaultUnit;

			/**	\brief	The maximum available size of the memory.
			 *
			 *			Given by: `(1 << bit_width) - 1`
			 */
			size_t max_size;

			/**	\brief	The maximum usable address in the memory.
			 *
			 *			Given by `mem_size - 1`
			 */
			std::bitset<bit_width> max_address;

		public:
			/**	\brief	Default constructor
			 *
			 *	\param	defaultUnit
			 *		The default unit to display data in ( `MemoryUnits::KILOBYTES` ).
			 *	\exception	Exceptions::Exception
			 *		Throws exception if mem_size is zero or larger than the highest possible address (undefined behaviour).
			 */
			Memory(MemoryUnits defaultUnit = MemoryUnits::KILOBYTES)
				: defaultUnit(defaultUnit), max_size((1 << bit_width) - 1), max_address(std::bitset<bit_width>(mem_size - 1)) {
				#ifdef THROW_EXCEPTIONS
					if (!mem_size)
						throw Exceptions::Exception("[ERROR] Memory size is zero!");
					if (mem_size - 1 > this->getMaxSize())
						throw Exceptions::Exception("[ERROR] Memory size is to big: Insufficient adresses!");
				#endif
				LockBlock lock(this);

				this->_memory = SysUtils::allocArray<std::bitset<bit_width>>(mem_size);
			}

			/**	\brief	Default destructor
			 *
			 *			Deletes internal memory array.
			 */
			~Memory() {
				delete this->_memory;
			}

			/**	\brief	Return the size of the memory expressed in a given MemoryUnit.
			 */
			float getSize(MemoryUnits unit) const {
				float size = bit_width * mem_size;

				switch (unit) {
					case MemoryUnits::BYTES:		return size / 8;		// size >> 3
					case MemoryUnits::KILOBYTES:	return size / 8192;		// size >> 13
					case MemoryUnits::MEGABYTES:	return size / 8388608;	// size >> 23
					case MemoryUnits::BITS:
					default:						return size;
				}
			}

			/**	\brief	Return the size of the memory expressed in the defautlt MemoryUnit.
			 */
			float getSize(void) const {
				return this->getSize(this->defaultUnit);
			}

			/**	\brief	Returns the maximum memory size.
			*/
			size_t getMaxSize(void) const {
				return this->max_size;
			}

			/**	\brief	Returns the maximum usable memory address.
			*/
			std::bitset<bit_width> getMaxAddress(void) const {
				return this->max_address;
			}

			/**	\brief	Returns the default MemoryUnuit.
			 */
			MemoryUnits getSizeUnit(void) const {
				return this->defaultUnit;
			}

			/**	\brief	Get data from memory.
			 *
			 *	\param	address
			 *		The address from which to return the data.
			 *		Must be smaller than the maximum address.
			 *
			 *	\return	std::bitset<bit_width>
			 *		Returns the bitset data on the given address
			 *
			 *	\exception	Exceptions::OutOfBoundsException
			 *		Throws exception if address is larger than the highest possible address (undefined behaviour).
			 */
			std::bitset<bit_width>& getData(std::bitset<bit_width> address) {
				#ifdef THROW_EXCEPTIONS
					if (address.to_ulong() > this->getMaxAddress().to_ulong())
						//throw Exceptions::OutOfBoundsException("[ERROR] Memory address out of bounds!");
						throw Exceptions::OutOfBoundsException(address.to_ulong());
				#endif
				LockBlock lock(this);
				return this->_memory[address.to_ullong()];
			}

			/**	\brief	Get data range from memory.
			 *
			 *	\param	from
			 *		The address from which to start.
			 *		Must be smaller than the maximum address and `to`.
			 *
			 *	\param	to
			 *		The address where to stop.
			 *		Must be smaller than the maximum address and bigger or equal to `from`.
			 *
			 *	\return	std::bitset<bit_width>*
			 *		Returns an array with the bitset data from `from` to `to`.
			 *
			 *	\exception	Exceptions::OutOfBoundsException
			 *		Throws exception if to is larger than the highest possible address (undefined behaviour).
			 *	\exception	Exceptions::OutOfBoundsException
			 *		Throws exception if from is larger than to (undefined behaviour).
			 */
			std::bitset<bit_width>* getDataRange(std::bitset<bit_width> from, std::bitset<bit_width> to) {
				#ifdef THROW_EXCEPTIONS
					if (to.to_ulong() > this->getMaxAddress().to_ulong())
						throw Exceptions::OutOfBoundsException(to.to_ulong());
					if (from.to_ulong() > to.to_ulong())
						throw Exceptions::OutOfBoundsException(from.to_ulong());
				#endif
				LockBlock lock(this);
				std::bitset<bit_width>* range = SysUtils::allocArray<std::bitset<bit_width>>(to.to_ulong() - from.to_ulong());

				for (size_t i = from.to_ulong(); i <= to.to_ulong(); ++i)
					range[i - from.to_ulong()] = this->_memory[i];

				return range;
			}

			/**	\brief	Set data on a given address in the memory.
			 *
			 *	\param	address
			 *		The address where to put data.
			 *		Must be smaller than the maximum address.
			 *
			 *	\param	data
			 *		The bitset data to add.
			 *
			 *	\exception	Exceptions::OutOfBoundsException
			 *		Throws exception if address is larger than the highest possible address (undefined behaviour).
			 */
			void setData(std::bitset<bit_width> address, std::bitset<bit_width> data) {
				#ifdef THROW_EXCEPTIONS
					if (address.to_ulong() > this->getMaxAddress().to_ulong())
						//throw Exceptions::OutOfBoundsException("[ERROR] Memory address out of bounds!");
						throw Exceptions::OutOfBoundsException(address.to_ulong());
				#endif
				LockBlock lock(this);
				this->_memory[address.to_ulong()] = data;
			}

			/**	\brief	Reset data on a given address in the memory (nullify).
			 *
			 *	\param	address
			 *		The address where to put data.
			 *		Must be smaller than the maximum address.
			 *
			 *	\exception	Exceptions::OutOfBoundsException
			 *		Throws exception if address is larger than the highest possible address (undefined behaviour).
			 */
			void resetData(std::bitset<bit_width> address) {
				#ifdef THROW_EXCEPTIONS
					if (address.to_ulong() > this->getMaxAddress().to_ulong())
						//throw Exceptions::OutOfBoundsException("[ERROR] Memory address out of bounds!");
						throw Exceptions::OutOfBoundsException(address.to_ulong());
				#endif
				LockBlock lock(this);
				this->_memory[address.to_ulong()].reset();
			}

			/**	\brief	Add the memory size and unit to a given stream (`os << Memory`).
			 */
			friend std::ostream& operator<<(std::ostream &os, Memory& m) {
				os << m.getSize(m.getSizeUnit()) << " " << memoryUnitToString(m.getSizeUnit() /*, true */);
				return os;
			}

			/**	\brief	Add the memory size and unit to a given stream (`os << Memory`).
			 */
			friend std::ostream& operator<<(std::ostream &os, Memory *m) {
				return os << *m;
			}
	};
}

#endif // MEMORY_HPP
