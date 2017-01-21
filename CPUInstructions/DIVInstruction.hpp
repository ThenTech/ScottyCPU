#ifndef DIVINSTRUCTION_HPP
#define DIVINSTRUCTION_HPP

#include "Instruction.hpp"
#include "../CPUComponents/DIVIDE.hpp"


namespace CPUInstructions {

	/**
	 *	\brief	**DIVInstruction**: DIV inputs and set flags.
	 *
	 *	Changeable `FLAGS`:
	 *
	 *	    Zero, Negative
	 *
	 *	\tparam	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class DIVInstruction : public CPUComponents::DIVIDE<bit_width>, public FlagRegister {
		private:

		public:
			/**
			 *	Default constructor
			 */
			DIVInstruction() : CPUComponents::DIVIDE<bit_width>(0u) {}

			/**
			 *	Default destructor
			 */
			~DIVInstruction() {}

			/**	\brief	The tick() method will be called when one of this Gate's inputs issues an emit().
			 *
			 *	\exception	Exceptions::Exception
			 *		Throws exception if less than 2 inputs are connected (undefined behaviour).
			 *	\exception	Exceptions::DivideByZeroException
			 *		Throws a hidden exception if second input is zero (output will be all 1s).
			 */
			void tick(void) {
				#ifdef THROW_EXCEPTIONS
					if (this->getInputs().size() < 2)
						throw Exceptions::Exception("[ERROR] DIV requires at least 2 inputs!");
				#endif
				this->clearFlags();

				//CPUComponents::DIVIDE<bit_width>::tick();
				std::bitset<bit_width> prevState = this->state;

				//this->state.reset();	// Default non-destructive state for DIVIDE-operation
				std::FloatingBitset<bit_width + 1> current(0.0);

				try {
					for(auto& connection : this->getInputs()) {
						if (connection == *this->getInputs().begin())
							current = connection->getState().to_ullong();
						else
							//current /= std::FloatingBitset<current.size()>(connection->getState().to_ullong());
							// MSVC fixed:
							current /= std::FloatingBitset<bit_width + 1>(connection->getState().to_ullong());
					}
				} catch (Exceptions::DivideByZeroException&) {
					current.set();
					this->setFlag(FLAGS::DivByZero);
				}

				this->state = current.to_ullong();

				if (this->state.none())
					this->setFlag(FLAGS::Zero);
				if (current.test(current.size() - 1))
					this->setFlag(FLAGS::Negative);
				if (current.to_ullong() >= this->getMSBMask<bit_width>())
					this->setFlag(FLAGS::CarryOut);

				if (prevState != this->state) this->emit();
			}
	};

}

#endif // DIVINSTRUCTION_HPP
