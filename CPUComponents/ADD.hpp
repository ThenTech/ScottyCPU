#ifndef ADD_H
#define ADD_H

#include "../SynchrotronComponent.hpp"
#include "../SignedBitset.hpp"
#include "../Exceptions.hpp"
using namespace Synchrotron;

namespace CPUComponents {

	/** \brief	**ADD** : ADD all inputs together.
	 *
	 *	\tparam	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class ADD : public SynchrotronComponent<bit_width> {
		public:
			/**
			 *	Default constructor
			 */
			ADD(size_t initial_value = 0) : SynchrotronComponent<bit_width>(initial_value) {}

			/**	Copy constructor
			 *	\param	other
			 *		SynchrotronComponent to copy from
			 *	\param	duplicateAll_IO
			 *		Specifies whether to only copy inputs (false) or outputs as well (true).
			 */
			ADD(const SynchrotronComponent<bit_width>& other, bool duplicateAll_IO = false)
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
			ADD(std::initializer_list<SynchrotronComponent<bit_width>*> inputList,
					std::initializer_list<SynchrotronComponent<bit_width>*> outputList = {} )
									: SynchrotronComponent<bit_width>(inputList, outputList) {}

			/**
			 *	Default destructor
			 */
			~ADD() {}

			/**	\brief	The tick() method will be called when one of this Gate's inputs issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if less than 2 inputs are connected (undefined behaviour).
			 */
			virtual void tick(void) {
				#ifdef THROW_EXCEPTIONS
					if (this->getInputs().size() < 2)
						throw Exceptions::Exception("[ERROR] ADD requires at least 2 inputs!");
				#endif
				std::bitset<bit_width> prevState = this->state;

				//this->state.reset();	// Default non-destructive state for ADD-operation
				std::SignedBitset<bit_width> current;

				for(auto& connection : this->getInputs()) {
					current += std::SignedBitset<bit_width>(connection->getState().to_ullong());
				}

				this->state = current.to_ullong();

				if (prevState != this->state) this->emit();
			}
	};
}

#endif // ADD_H
