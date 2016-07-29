#ifndef NOTGATE_HPP
#define NOTGATE_HPP

#include "SynchrotronComponent.hpp"
#include "Exceptions.hpp"
using namespace Synchrotron;

namespace CPUComponents {

	/** \brief	**NOTGate** : NOT all bits of it's input together.
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class NOTGate : public SynchrotronComponent<bit_width> {
		public:
			/**
			 *	Default constructor
			 */
			NOTGate(size_t initial_value = 0) : SynchrotronComponent<bit_width>(initial_value) {}

			/**	Copy constructor
			 *	\param	Other
			 *		SynchrotronComponent to copy from
			 *	\param	duplicateAll_IO
			 *		Specifies whether to only copy inputs (false) or outputs as well (true).
			 */
			NOTGate(const SynchrotronComponent<bit_width>& other, bool duplicateAll_IO = false)
				: SynchrotronComponent<bit_width>(other, duplicateAll_IO) {}

			/**
			 *	Default destructor
			 */
			~NOTGate() {}

			/**	\brief	The tick() method will be called when this Gate's input issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if more than 1 input is connected (undefined behaviour).
			 */
			inline void tick() {
				#ifdef THROW_EXCEPTIONS
					if (this->getIputs().size() > 1)
						throw Exceptions::Exception("[ERROR] NOTGate can have at most 1 input!");
				#endif
				std::bitset<bit_width> prevState = this->state;

				this->state = (*this->getIputs().begin())->getState().flip();

				if (prevState != this->state) this->emit();
			}
	};
}

#endif // NOTGATE_HPP
