#ifndef ANDGATE_HPP
#define ANDGATE_HPP

#include "SynchrotronComponent.hpp"
#include "Exceptions.hpp"
using namespace Synchrotron;

namespace CPUComponents {

	template <size_t bit_width>
	class ANDGate : public SynchrotronComponent<bit_width> {
		public:
			/**
			 *	Default constructor
			 */
			ANDGate(size_t initial_value = 0) : SynchrotronComponent<bit_width>(initial_value) {}

			/**	Copy constructor
			 *	\param	Other
			 *		SynchrotronComponent to copy from
			 *	\param	duplicateAll_IO
			 *		Specifies whether to only copy inputs (false) or outputs as well (true).
			 */
			ANDGate(const SynchrotronComponent<bit_width>& other, bool duplicateAll_IO = false)
				: SynchrotronComponent<bit_width>(other, duplicateAll_IO) {}

			/**
			 *	Default destructor
			 */
			~ANDGate() {}

			inline void tick() {
				#ifdef THROW_EXCEPTIONS
					if (this->getIputs().size() < 2)
						throw Exceptions::Exception("[ERROR] ANDGate requires at least 2 inputs!");
				#endif
				std::bitset<bit_width> prevState = this->state;

				this->state.set();	// Default non-destructive state for AND-operation

				for(auto& connection : this->getIputs()) {
					this->state &= connection->getState();
				}

				if (prevState != this->state) this->emit();
			}
	};
}

#endif // ANDGATE_HPP