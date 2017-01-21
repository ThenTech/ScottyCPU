#ifndef SYNCHROTRONCOMPONENTENABLE_HPP
#define SYNCHROTRONCOMPONENTENABLE_HPP

#include "SynchrotronComponentFixedInput.hpp"
#include "Exceptions.hpp"
using namespace Synchrotron;

namespace Synchrotron {

	class SynchrotronComponentEnable : public SynchrotronComponentFixedInput<1u, 1u> {
		public:
			/**
			 *	Default constructor
			 */
			SynchrotronComponentEnable(size_t initial_value = 0) : SynchrotronComponentFixedInput<1u, 1u>(initial_value) {}

			/**	Copy constructor
			 *	\param	other
			 *		SynchrotronComponent to copy from
			 *	\param	duplicateAll_IO
			 *		Specifies whether to only copy inputs (false) or outputs as well (true).
			 */
			SynchrotronComponentEnable(const SynchrotronComponent<1u>& other, bool duplicateAll_IO = false)
				: SynchrotronComponentFixedInput<1u, 1u>(other, duplicateAll_IO) {}

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
			SynchrotronComponentEnable(	std::initializer_list<SynchrotronComponent<1u>*> inputList,
										std::initializer_list<SynchrotronComponent<1u>*> outputList = {} )
									: SynchrotronComponentFixedInput<1u, 1u>(inputList, outputList) {}

			/**
			 *	Default destructor
			 */
			~SynchrotronComponentEnable() {}

			void setEnable(SynchrotronComponent& input) {
				this->enable = &input;
			}

//			void setEnable(SynchrotronComponent& input) {
//				this->enable = &input;
//			}
	};
}

#endif // SYNCHROTRONCOMPONENTENABLE_HPP
