#ifndef SUBTRACT_HPP
#define SUBTRACT_HPP

#include "../SynchrotronComponent.hpp"
#include "../SignedBitset.hpp"
#include "../Exceptions.hpp"
using namespace Synchrotron;

namespace CPUComponents {

	/** \brief	**SUBTRACT** : SUBTRACT all inputs.
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class SUBTRACT : public SynchrotronComponent<bit_width> {
		public:
			/**
			 *	Default constructor
			 */
			SUBTRACT(size_t initial_value = 0) : SynchrotronComponent<bit_width>(initial_value) {}

			/**	Copy constructor
			 *	\param	Other
			 *		SynchrotronComponent to copy from
			 *	\param	duplicateAll_IO
			 *		Specifies whether to only copy inputs (false) or outputs as well (true).
			 */
			SUBTRACT(const SynchrotronComponent<bit_width>& other, bool duplicateAll_IO = false)
				: SynchrotronComponent<bit_width>(other, duplicateAll_IO) {}

			/**	\brief
			 *	Connection constructor
			 *	*	Adds signal subscriptions from inputList
			 *	*	Optionally adds slot subscribers from outputList
			 *
			 *	\param	inputList
			 *		The list of SynchrotronComponents to connect as input.
			 *	\param	outputList
			 *		The list of SynchrotronComponents to connect as output.
			 */
			SUBTRACT(std::initializer_list<SynchrotronComponent<bit_width>*> inputList,
					 std::initializer_list<SynchrotronComponent<bit_width>*> outputList = {} )
										 : SynchrotronComponent<bit_width>(inputList, outputList) {}

			/**
			 *	Default destructor
			 */
			~SUBTRACT() {}

			/**	\brief	The tick() method will be called when one of this Gate's inputs issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if less than 2 inputs are connected (undefined behaviour).
			 */
			virtual void tick(void) {
				#ifdef THROW_EXCEPTIONS
					if (this->getInputs().size() < 2)
						throw Exceptions::Exception("[ERROR] SUBTRACT requires at least 2 inputs!");
				#endif
				std::bitset<bit_width> prevState = this->state;

				// Default non-destructive state for SUBTRACT-operation
				std::SignedBitset<bit_width> current;

				for(auto& connection : this->getInputs()) {
					if (connection == *this->getInputs().begin())
						current = connection->getState().to_ullong();
					else
						current -= std::SignedBitset<bit_width>(connection->getState().to_ullong());
				}

				this->state = current.to_ullong();

				if (prevState != this->state) this->emit();
			}
	};
}

#endif // SUBTRACT_HPP
