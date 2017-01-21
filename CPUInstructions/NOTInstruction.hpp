#ifndef NOTINSTRUCTION_HPP
#define NOTINSTRUCTION_HPP

#include "Instruction.hpp"
#include "../CPUComponents/NOTGate.hpp"


namespace CPUInstructions {

	/**
	 *	\brief	**NOTInstruction** : NOT inputs and set flags.
	 *
	 *	Changeable `FLAGS`:
	 *
	 *	    Zero, Negative
	 *
	 *	\tparam	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class NOTInstruction : public CPUComponents::NOTGate<bit_width>, public FlagRegister {
		private:

		public:
			/**
			 *	Default constructor
			 */
			NOTInstruction() : CPUComponents::NOTGate<bit_width>(0u) {}

			/**
			 *	Default destructor
			 */
			~NOTInstruction() {}

			/**	\brief	The tick() method will be called when one of this Gate's inputs issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if more than 1 input is connected (undefined behaviour).
			 */
			void tick(void) {
				this->clearFlags();

				CPUComponents::NOTGate<bit_width>::tick();

				this->evaluateResultConditions(this->getState());
			}
	};

}

#endif // NOTINSTRUCTION_HPP
