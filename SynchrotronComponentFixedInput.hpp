#ifndef SYNCHROTRONCOMPONENTFIXEDINPUT_HPP
#define SYNCHROTRONCOMPONENTFIXEDINPUT_HPP


#include "SynchrotronComponent.hpp"
#include "Exceptions.hpp"
using namespace Synchrotron;

namespace Synchrotron {

	/** \brief	**SynchrotronComponentFixedInput** : SynchrotronComponent with limited inputs.
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 *	\param	max_inputs
	 *		This template argument specifies the maximum amount of input connections.
	 */
	template <size_t bit_width, size_t max_inputs>
	class SynchrotronComponentFixedInput : public SynchrotronComponent<bit_width> {
		public:
			/**
			 *	Default constructor
			 */
			SynchrotronComponentFixedInput(size_t initial_value = 0) : SynchrotronComponent<bit_width>(initial_value) {}

			/**	\brief **[Thread safe]**
			 *	Copy constructor
			 *	*	Duplicates signal subscriptions (inputs)
			 *	*	Optionally also duplicates slot connections (outputs)
			 *
			 *	\param	sc const
			 *		The other SynchrotronComponent to duplicate the connections from.
			 *	\param	duplicateAll_IO
			 *		Specifies whether to only copy inputs (false) or outputs as well (true).
			 */
			SynchrotronComponentFixedInput(const SynchrotronComponent<bit_width>& sc, bool duplicateAll_IO = false) : SynchrotronComponentFixedInput<bit_width, 1u>() {
				//LockBlock lock(this);

				// Copy subscriptions
				for(auto& sender : sc.getInputs()) {
					this->addInput(*sender);
				}

				if (duplicateAll_IO) {
					// Copy subscribers
					for(auto& connection : sc.getOutputs()) {
						this->addOutput(*connection);
					}
				}
			}

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
			SynchrotronComponentFixedInput(	std::initializer_list<SynchrotronComponent<bit_width>*> inputList,
											std::initializer_list<SynchrotronComponent<bit_width>*> outputList = {} )
									: SynchrotronComponentFixedInput<bit_width, 1u>() {
				this->addInput(inputList);
				this->addOutput(outputList);
			}

			/**
			 *	Default destructor
			 */
			~SynchrotronComponentFixedInput() {}

			/**	\brief	Gets this SynchrotronComponent's max inputs.
			 *
			 *	\return	size_t
			 *      Returns the maximum amounts of inputs.
			 */
			inline size_t getMaxInputs() {
				return max_inputs;
			}

			/**	\brief	Gets the SynchrotronComponentFixedInput's input connection.
			 *
			 *	\return	SynchrotronComponent&
			 *      Returns a reference set to this SynchrotronComponent's input.
			 */
			const SynchrotronComponent<bit_width>& getInput() {
				return **this->getInputs().begin();
			}

			/**	\brief	**[Thread safe]** Adds/Connects a new input to this SynchrotronComponent.
			 *
			 *	**Ensures both way connection will be made:**
			 *	This will have input added to its inputs and input will have this added to its outputs.
			 *
			 *	\param	input
			 *		The SynchrotronComponent to connect as input.
			 *	\exception	Exceptions::Exception
			 *		Throws exception if getInputs() already contains max_inputs.
			 */
			void addInput(SynchrotronComponent<bit_width>& input) {
				if (this->getInputs().size() >= max_inputs) {
					#ifdef THROW_EXCEPTIONS
						throw Exceptions::Exception("[SynchrotronComponentFixedInput] This component already has its required inputs!");
					#endif
					return;
				}

				SynchrotronComponent<bit_width>::addInput(input);
			}

			/**	\brief	Adds/Connects a list of new inputs to this SynchrotronComponent.
			 *
			 *	Calls addInput() on each SynchrotronComponent* in inputList.
			 *
			 *	\param	inputList
			 *		The list of SynchrotronComponents to connect as input.
			 */
			void addInput(std::initializer_list<SynchrotronComponent<bit_width>*> inputList) {
				for(auto connection : inputList)
					this->addInput(*connection);
			}
	};
}

#endif // SYNCHROTRONCOMPONENTFIXEDINPUT_HPP
