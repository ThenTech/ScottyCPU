#ifndef CMP_HPP
#define CMP_HPP

#include "Instruction.hpp"
#include "../CPUComponents/COMPERATOR.hpp"


namespace CPUInstructions {

	/**
	 *	\brief	**CMPInstruction** : CMP inputs and set flags
	 *
	 *	Changeable FLAGS:
	 *		Zero, Equal, Negative, Smaller, Larger
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class CMPInstruction : public CPUComponents::COMPERATOR<bit_width>, public FlagRegister {
		private:

		public:
			/**
			 *	Default constructor
			 */
			CMPInstruction() : CPUComponents::COMPERATOR<bit_width>(0u) {}

			/**
			 *	Default destructor
			 */
			~CMPInstruction() {}

			/**	\brief	The tick() method will be called when one of this Gate's inputs issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if less than 2 inputs are connected (undefined behaviour).
			 */
			void tick(void) {
				#ifdef THROW_EXCEPTIONS
					if (this->getInputs().size() < 2)
						throw Exceptions::Exception("[ERROR] CMP requires exactly 2 inputs!");
				#endif
				this->clearFlags();

				//CPUComponents::COMPERATOR<bit_width>::tick();
				std::bitset<bit_width> prevState = this->state;

				//this->state.reset();	// Default non-destructive state for COMPERATOR-operation
				std::SignedBitset<bit_width + 1> current(0);

				for(auto& connection : this->getInputs()) {
					if (connection == *this->getInputs().begin())
						current = connection->getState().to_ullong();
					else
						this->state = (current = current.compareTo(std::SignedBitset<bit_width + 1>(connection->getState().to_ullong()))).to_ullong();
				}

				if (this->state.none()) {
					this->setFlag(FLAGS::Zero);
					this->setFlag(FLAGS::Equal);
				}
				if (current.sign() < 0) {
					this->setFlag(FLAGS::Negative);
					this->setFlag(FLAGS::Smaller);
				}
				if (current.sign() > 0)
					this->setFlag(FLAGS::Larger);

				if (prevState != this->state) this->emit();
			}
	};

}

#endif // CMP_HPP
