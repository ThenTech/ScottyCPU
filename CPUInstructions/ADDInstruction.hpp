#ifndef ADDINSTRUCTION_HPP
#define ADDINSTRUCTION_HPP

#include "Instruction.hpp"
#include "../CPUComponents/ADD.hpp"


namespace CPUInstructions {

	/**
	 *	\brief	**ADDInstruction** : ADD inputs and set flags.
	 *
	 *	Changeable `FLAGS`:
	 *
	 *	    Zero, Negative, CarryOut
	 *
	 *	\tparam	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class ADDInstruction : public CPUComponents::ADD<bit_width>, public FlagRegister {
		private:

		public:
			/**
			 *	Default constructor
			 */
			ADDInstruction() : CPUComponents::ADD<bit_width>(0u) {}

			/**
			 *	Default destructor
			 */
			~ADDInstruction() {}

			/**	\brief	The tick() method will be called when one of this Gate's inputs issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if less than 2 inputs are connected (undefined behaviour).
			 */
			void tick(void) {
				#ifdef THROW_EXCEPTIONS
					if (this->getInputs().size() < 2)
						throw Exceptions::Exception("[ERROR] ADD requires at least 2 inputs!");
				#endif
				this->clearFlags();

				//CPUComponents::ADD<bit_width>::tick();
				std::bitset<bit_width> prevState = this->state;

				//this->state.reset();	// Default non-destructive state for ADD-operation
				std::SignedBitset<bit_width + 1> current(0);

				for(auto& connection : this->getInputs()) {
					//current += std::SignedBitset<current.size()>(connection->getState().to_ullong());
					// MSVC fixed:
					current += std::SignedBitset<bit_width + 1>(connection->getState().to_ullong());
				}

				this->state = current.to_ullong();

				if (this->state.none())
					this->setFlag(FLAGS::Zero);
				if (current.test(bit_width))
					this->setFlag(FLAGS::Negative);
				if (current.to_ullong() >= this->getMSBMask<bit_width>())
					this->setFlag(FLAGS::CarryOut);

				if (prevState != this->state) this->emit();
			}
	};

}

#endif // ADDINSTRUCTION_HPP
