#ifndef GATE_H
#define GATE_H

#include <bitset>
#include "SynchronousComponent.h"

namespace CPUComponents {

	template <size_t width>
	class Gate : public SynchronousComponent {
		protected:
			std::bitset<width>	*in;
			std::bitset<1u>	out;
		public:
			Gate(std::bitset<width> *const input = nullptr) {
				this->in = input;
			}

			virtual ~Gate() {
				// Don't delete this->in:
				// Pointer space not managed by this class, but by class providing input.
			}

			//virtual void tick();

			std::bitset<1u> output() {
				return this->out;
			}

			std::bitset<1u>* outRef() {
				return &this->out;
			}
	};

}

#endif // GATE_H
