#ifndef SHLINSTRUCTION_HPP
#define SHLINSTRUCTION_HPP

#include "Instruction.hpp"
#include "../CPUComponents/SHIFTLeft.hpp"


namespace CPUInstructions {

	/**
	 *	\brief	**SHLInstruction** : SHL inputs and set flags.
	 *
	 *	Changeable FLAGS:
	 *		Zero, CarryOut
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class SHLInstruction : public CPUComponents::SHIFTLeft<bit_width>, public FlagRegister {
		private:

		public:
			/**
			 *	Default constructor
			 */
			SHLInstruction() : CPUComponents::SHIFTLeft<bit_width>(0u) {}

			/**
			 *	Default destructor
			 */
			~SHLInstruction() {}

			/**	\brief	The tick() method will be called when one of this Gate's inputs issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if no inputs are connected (undefined behaviour).
			 */
			// TO-DO: change SHIFTLeft to 2 inputs with 2nd arg is amount to shift?
			void tick(void) {
				#ifdef THROW_EXCEPTIONS
					if (this->getInputs().size() == 0)
						throw Exceptions::Exception("[ERROR] SHIFTLeft requires 1 input!");
				#endif
				this->clearFlags();

				//CPUComponents::SHIFTLeft<bit_width>::tick();
				std::bitset<bit_width> prevState = this->state;

				std::bitset<bit_width + 1> current(this->getInput().getState().to_ullong());

				current <<= 1;

				this->state = current.to_ullong();

				if (this->state.none())
					this->setFlag(FLAGS::Zero);
				if (current.to_ullong() >= this->getMSBMask<bit_width>())
					this->setFlag(FLAGS::CarryOut);

				if (prevState != this->state) this->emit();
			}
	};

}

#endif // SHLINSTRUCTION_HPP
