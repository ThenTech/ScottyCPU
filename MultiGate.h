#ifndef MULTIGATE_H
#define MULTIGATE_H

#include <bitset>
#include <vector>
#include "SynchronousComponent.h"
#include "Exceptions.h"

namespace CPUComponents {

	/*
	 *	A gate with <sets> amounts of <width> wide inputs.
	 *
	 *	e.g.
	 *		MultiGate<1, 8>	:	1 input, 8 bits wide
	 *		MultiGate<8, 1>	:	8 inputs, each 1 bit wide
	 */
	template <size_t sets, size_t width>
	class MultiGate : public SynchronousComponent {
		protected:
			std::bitset<width>					*in	= nullptr;
			std::vector<std::bitset<width>*>	multi_in;

			std::bitset<1u>						out;
			std::bitset<width>					multi_out;
		public:
			MultiGate(std::bitset<width> *const input) {
				if (sets > 1)
					throw Exceptions::OutOfBoundsException(sets);
				this->in = input;
			}

			MultiGate(std::bitset<width> *const *input = nullptr) {
				if (!sets)
					throw Exceptions::OutOfBoundsException(sets);

				if (sets == 1) {
					if (input != nullptr)
						this->addInput(input[0]);
				} else {
					if (input != nullptr)
						for (size_t i = 0; i < sets; i++)
							this->addInput(input[i]);
					else
						multi_in.reserve(sets);
				}
			}

			virtual ~MultiGate() {
				// Don't delete this->in:
				// Pointer space not managed by this class, but by scope providing input.
			}

			//virtual void tick();	// Implement in child class to change out or multi_out.

			void addInput(std::bitset<width> *const input) {
				if (this->allInputsSet())
					throw Exceptions::Exception("Gate already has all its inputs set!");

				if (sets < 2)
					this->in = input;	// Warning: in pointer overwritten every call.
				else
					this->multi_in.push_back(input);
			}

			std::bitset<width>* getInput(size_t id = 0) { // id = [0, sets-1]
				if (id >= sets || (sets > 1 && (id + 1) > this->multi_in.size()))
					throw Exceptions::OutOfBoundsException(id);
				return sets == 1 ? in : multi_in.at(id);
			}

			size_t getInputAmount() {
				return sets;
			}

			bool allInputsSet() {
				return (sets == 1 && this->in != nullptr) || (this->multi_in.size() == sets);
			}

			std::bitset<1u> getOutput() {
//				if (sets > 1)
//					throw Exceptions::Exception("Multiple outputs generated! Use getMultiOutput() instead.");
				return this->out;
			}

			std::bitset<1u>* getOutRef() {
//				if (sets > 1)
//					throw Exceptions::Exception("Multiple outputs generated! Use getMultiOutRef() instead.");
				return &this->out;
			}

			std::bitset<width> getMultiOutput() {
//				if (sets < 2)
//					throw Exceptions::Exception("Single output generated! Use getOutput() instead.");
				return this->multi_out;
			}

			std::bitset<width>* getMultiOutRef() {
//				if (sets < 2)
//					throw Exceptions::Exception("Single output generated! Use getOutRef() instead.");
				return &this->multi_out;
			}
	};

}

#endif // MULTIGATE_H
