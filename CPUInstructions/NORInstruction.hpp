#ifndef NORINSTRUCTION_HPP
#define NORINSTRUCTION_HPP

#include "Instruction.hpp"
#include "../CPUComponents/NORGate.hpp"


namespace CPUInstructions {

	/**
	 *	\brief	**NORInstruction** : NOR inputs and set flags.
	 *
	 *	Changeable FLAGS:
	 *		Zero, Negative
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class NORInstruction : public CPUComponents::NORGate<bit_width>, public FlagRegister {
		private:

		public:
			/**
			 *	Default constructor
			 */
			NORInstruction() : CPUComponents::NORGate<bit_width>(0u) {}

			/**
			 *	Default destructor
			 */
			~NORInstruction() {}

			/**	\brief	The tick() method will be called when one of this Gate's inputs issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if less than 2 inputs are connected (undefined behaviour).
			 */
			void tick(void) {
				this->clearFlags();

				CPUComponents::NORGate<bit_width>::tick();

				this->evaluateResultConditions(this->getState());
			}
	};

}

#endif // NORINSTRUCTION_HPP
