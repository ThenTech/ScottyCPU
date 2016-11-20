#ifndef SHRINSTRUCTION_HPP
#define SHRINSTRUCTION_HPP

#include "Instruction.hpp"
#include "../CPUComponents/SHIFTRight.hpp"


namespace CPUInstructions {

	/**
	 *	\brief	**SHRInstruction** : SHR inputs and set flags.
	 *
	 *	Changeable FLAGS:
	 *		Zero
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class SHRInstruction : public CPUComponents::SHIFTRight<bit_width>, public FlagRegister {
		private:

		public:
			/**
			 *	Default constructor
			 */
			SHRInstruction() : CPUComponents::SHIFTRight<bit_width>(0u) {}

			/**
			 *	Default destructor
			 */
			~SHRInstruction() {}

			/**	\brief	The tick() method will be called when one of this Gate's inputs issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if no inputs are connected (undefined behaviour).
			 */
			// TO-DO: change SHIFTRight to 2 inputs with 2nd arg is amount to shift?
			void tick(void) {
				this->clearFlags();

				CPUComponents::SHIFTRight<bit_width>::tick();

				if (this->getState().none())
					this->setFlag(FLAGS::Zero);
			}
	};

}

#endif // SHRINSTRUCTION_HPP
