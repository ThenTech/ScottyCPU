#ifndef SHIFTRight_HPP
#define SHIFTRight_HPP

#include "SynchrotronComponentFixedInput.hpp"
#include "Exceptions.hpp"
using namespace Synchrotron;

namespace CPUComponents {

	/** \brief	**NOTGate** : NOT all bits of it's input together.
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class SHIFTRight : public SynchrotronComponentFixedInput<bit_width, 1u> {
		public:
			/**
			 *	Default constructor
			 */
			SHIFTRight(size_t initial_value = 0) : SynchrotronComponentFixedInput<bit_width, 1u>(initial_value) {}

			/**	Copy constructor
			 *	\param	Other
			 *		SynchrotronComponent to copy from
			 *	\param	duplicateAll_IO
			 *		Specifies whether to only copy inputs (false) or outputs as well (true).
			 */
			SHIFTRight(const SynchrotronComponent<bit_width>& other, bool duplicateAll_IO = false)
				: SynchrotronComponentFixedInput<bit_width, 1u>(other, duplicateAll_IO) {}

			/**	\brief
			 *	Connection constructor
			 *	*	Adds signal subscriptions from inputList
			 *	*	Optionally adds slot subscribers from outputList
			 *
			 *	\param	inputList
			 *		The list of SynchrotronComponents to connect as input.
			 *	\param	outputList
			 *		The list of SynchrotronComponents to connect as output..
			 */
			SHIFTRight(std::initializer_list<SynchrotronComponent<bit_width>*> inputList,
					std::initializer_list<SynchrotronComponent<bit_width>*> outputList = {} )
									: SynchrotronComponentFixedInput<bit_width, 1u>(inputList, outputList) {}

			/**
			 *	Default destructor
			 */
			~SHIFTRight() {}

			/**	\brief
			 *		The tick() method will be called when this Gate's input issues an emit().
			 */
			inline void tick() {
				std::bitset<bit_width> prevState = this->state;

				this->state = this->getInput().getState() >> 1;

				if (prevState != this->state) this->emit();
			}
	};
}

#endif // SHIFTRight_HPP
