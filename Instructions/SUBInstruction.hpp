#ifndef SUBINSTRUCTION_HPP
#define SUBINSTRUCTION_HPP

#include "Instruction.hpp"
#include "../CPUComponents/SUBTRACT.hpp"


namespace CPUInstructions {

	/**
	 *	\brief	**SUBInstruction** : SUB inputs and set flags.
	 *
	 *	Changeable FLAGS:
	 *		Zero, Negative, CarryOut
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class SUBInstruction : public CPUComponents::SUBTRACT<bit_width>, public FlagRegister {
		private:

		public:
			/**
			 *	Default constructor
			 */
			SUBInstruction() : CPUComponents::SUBTRACT<bit_width>(0u) {}

			/**
			 *	Default destructor
			 */
			~SUBInstruction() {}

			/**	\brief	The tick() method will be called when one of this Gate's inputs issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if less than 2 inputs are connected (undefined behaviour).
			 */
			void tick(void) {
				#ifdef THROW_EXCEPTIONS
					if (this->getInputs().size() < 2)
						throw Exceptions::Exception("[ERROR] SUBTRACT requires at least 2 inputs!");
				#endif
				this->clearFlags();

				//CPUComponents::SUBTRACT<bit_width>::tick();
				std::bitset<bit_width> prevState = this->state;

				//this->state.reset();	// Default non-destructive state for SUBTRACT-operation
				std::SignedBitset<bit_width + 1> current;

				for(auto& connection : this->getInputs()) {
					if (connection == *this->getInputs().begin())
						current = connection->getState().to_ullong();
					else
						current -= std::SignedBitset<current.size()>(connection->getState().to_ullong());
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

#endif // SUBINSTRUCTION_HPP
