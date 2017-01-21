#ifndef ANDINSTRUCTION_HPP
#define ANDINSTRUCTION_HPP

#include "Instruction.hpp"
#include "../CPUComponents/ANDGate.hpp"


namespace CPUInstructions {

	/**
	 *	\brief	**ANDInstruction** : AND inputs and set flags.
	 *
	 *	Changeable `FLAGS`:
	 *
	 *	    Zero, Negative
	 *
	 *	\tparam	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class ANDInstruction : public CPUComponents::ANDGate<bit_width>, public FlagRegister {
		private:

		public:
			/**
			 *	Default constructor
			 */
			ANDInstruction() : CPUComponents::ANDGate<bit_width>(0u) {}

			/**
			 *	Default destructor
			 */
			~ANDInstruction() {}

			/**	\brief	The tick() method will be called when one of this Gate's inputs issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if less than 2 inputs are connected (undefined behaviour).
			 */
			void tick(void) {
				this->clearFlags();

				CPUComponents::ANDGate<bit_width>::tick();

				this->evaluateResultConditions(this->getState());
			}
	};

}

#endif // ANDINSTRUCTION_HPP
