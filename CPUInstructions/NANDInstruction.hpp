#ifndef NANDINSTRUCTION_HPP
#define NANDINSTRUCTION_HPP

#include "Instruction.hpp"
#include "../CPUComponents/NANDGate.hpp"


namespace CPUInstructions {

	/**
	 *	\brief	**NANDInstruction** : NAND inputs and set flags.
	 *
	 *	Changeable FLAGS:
	 *		Zero, Negative
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class NANDInstruction : public CPUComponents::NANDGate<bit_width>, public FlagRegister {
		private:

		public:
			/**
			 *	Default constructor
			 */
			NANDInstruction() : CPUComponents::NANDGate<bit_width>(0u) {}

			/**
			 *	Default destructor
			 */
			~NANDInstruction() {}

			/**	\brief	The tick() method will be called when one of this Gate's inputs issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if less than 2 inputs are connected (undefined behaviour).
			  */
			void tick(void) {
				this->clearFlags();

				CPUComponents::NANDGate<bit_width>::tick();

				this->evaluateResultConditions(this->getState());
			}
	};

}

#endif // NANDINSTRUCTION_HPP
