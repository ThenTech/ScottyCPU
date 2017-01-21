#ifndef MODULO_HPP
#define MODULO_HPP

#include "../SynchrotronComponentFixedInput.hpp"
#include "../SignedBitset.hpp"
#include "../Exceptions.hpp"
using namespace Synchrotron;

namespace CPUComponents {

	/** \brief	**MODULO** : Calculate the remainder after division.
	 *
	 *	\tparam	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class MODULO : public SynchrotronComponentFixedInput<bit_width, 2u> {
		public:
			/**
			 *	Default constructor
			 */
			MODULO(size_t initial_value = 0) : SynchrotronComponentFixedInput<bit_width, 2u>(initial_value) {}

			/**	Copy constructor
			 *	\param	other
			 *		SynchrotronComponent to copy from
			 *	\param	duplicateAll_IO
			 *		Specifies whether to only copy inputs (false) or outputs as well (true).
			 */
			MODULO(const SynchrotronComponent<bit_width>& other, bool duplicateAll_IO = false)
				: SynchrotronComponentFixedInput<bit_width, 1u>(other, duplicateAll_IO) {}

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
			MODULO(std::initializer_list<SynchrotronComponent<bit_width>*> inputList,
				   std::initializer_list<SynchrotronComponent<bit_width>*> outputList = {} )
									: SynchrotronComponentFixedInput<bit_width, 2u>(inputList, outputList) {}

			/**
			 *	Default destructor
			 */
			~MODULO() {}

			/**	\brief
			 *		The tick() method will be called when this Gate's input issues an emit().
			 */
			void tick(void) {
				#ifdef THROW_EXCEPTIONS
					if (this->getInputs().size() < 2)
						throw Exceptions::Exception("[ERROR] MODULO requires exactly 2 inputs!");
				#endif
				std::bitset<bit_width> prevState = this->state;

				//this->state.reset();	// Default non-destructive state for MODULO-operation
				std::SignedBitset<bit_width + 1> current(0);

				for(auto& connection : this->getInputs()) {
					if (connection == *this->getInputs().begin())
						current = connection->getState().to_ullong();
					else
						current %= std::SignedBitset<bit_width + 1>(connection->getState().to_ullong());
				}

				this->state = current.to_ullong();

				if (prevState != this->state) this->emit();
			}
	};
}

#endif // MODULO_HPP
