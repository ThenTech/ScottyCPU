#ifndef ORINSTRUCTION_H
#define ORINSTRUCTION_H

#include "Instruction.hpp"
#include "../CPUComponents/ORGate.hpp"


namespace CPUInstructions {

	/**
	 *	\brief	**ORInstruction** : OR inputs and set flags.
	 *
	 *	Changeable `FLAGS`:
	 *
	 *	    Zero, Negative
	 *
	 *	\tparam	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class ORInstruction : public CPUComponents::ORGate<bit_width>, public FlagRegister {
		private:

		public:
			/**
			 *	Default constructor
			 */
			ORInstruction() : CPUComponents::ORGate<bit_width>(0u) {}

			/**
			 *	Default destructor
			 */
			~ORInstruction() {}

			/**	\brief	The tick() method will be called when one of this Gate's inputs issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if less than 2 inputs are connected (undefined behaviour).
			 */
			void tick(void) {
				this->clearFlags();

				CPUComponents::ORGate<bit_width>::tick();

				this->evaluateResultConditions(this->getState());
			}
	};

}

#endif // ORINSTRUCTION_H
