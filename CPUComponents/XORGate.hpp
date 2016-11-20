#ifndef XORGATE_HPP
#define XORGATE_HPP

#include "../SynchrotronComponent.hpp"
#include "../Exceptions.hpp"
using namespace Synchrotron;

namespace CPUComponents {

	/** \brief	**XORGate** : XOR all inputs together.
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class XORGate : public SynchrotronComponent<bit_width> {
		public:
			/**
			 *	Default constructor
			 */
			XORGate(size_t initial_value = 0) : SynchrotronComponent<bit_width>(initial_value) {}

			/**	Copy constructor
			 *	\param	Other
			 *		SynchrotronComponent to copy from
			 *	\param	duplicateAll_IO
			 *		Specifies whether to only copy inputs (false) or outputs as well (true).
			 */
			XORGate(const SynchrotronComponent<bit_width>& other, bool duplicateAll_IO = false)
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
			XORGate(std::initializer_list<SynchrotronComponent<bit_width>*> inputList,
					std::initializer_list<SynchrotronComponent<bit_width>*> outputList = {} )
									: SynchrotronComponent<bit_width>(inputList, outputList) {}

			/**
			 *	Default destructor
			 */
			~XORGate() {}

			/**	\brief	The tick() method will be called when one of this Gate's inputs issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if less than 2 inputs are connected (undefined behaviour).
			 */
			void tick(void) {
				#ifdef THROW_EXCEPTIONS
					if (this->getInputs().size() < 2)
						throw Exceptions::Exception("[ERROR] XORGate requires at least 2 inputs!");
				#endif
				std::bitset<bit_width> prevState = this->state;

				this->state.reset();	// Default non-destructive state for XOR-operation

				for(auto& connection : this->getInputs()) {
					this->state ^= connection->getState();
				}

				if (prevState != this->state) this->emit();
			}
	};
}

#endif // XORGATE_HPP
