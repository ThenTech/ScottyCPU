#ifndef XORINSTRUCTION_HPP
#define XORINSTRUCTION_HPP

#include "Instruction.hpp"
#include "../CPUComponents/XORGate.hpp"


namespace CPUInstructions {

	/**
	 *	\brief	**XORInstruction** : XOR inputs and set flags.
	 *
	 *	Changeable FLAGS:
	 *		Zero, Negative
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class XORInstruction : public CPUComponents::XORGate<bit_width>, public FlagRegister {
		private:

		public:
			/**
			 *	Default constructor
			 */
			XORInstruction() : CPUComponents::XORGate<bit_width>(0u) {}

			/**
			 *	Default destructor
			 */
			~XORInstruction() {}

			/**	\brief	The tick() method will be called when one of this Gate's inputs issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if less than 2 inputs are connected (undefined behaviour).
			 */
			void tick(void) {
				this->clearFlags();

				CPUComponents::XORGate<bit_width>::tick();

				this->evaluateResultConditions(this->getState());
			}
	};

}

#endif // XORINSTRUCTION_HPP
