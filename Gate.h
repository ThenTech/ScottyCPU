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
				//this->in->reset();
				//this->out.reset();
				delete this->in;	// Delete input bitset pointer? Throws error in MSVC
			}

			//virtual void tick();

			std::bitset<1u> output() {
				return this->out;
			}
	};

}

#endif // GATE_H
