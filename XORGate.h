#ifndef XORGATE_H
#define XORGATE_H

#include "Gate.h"

namespace CPUComponents {

	template <size_t width>
	class XORGate : public Gate<width> {
		public:
			XORGate(std::bitset<width> *const input = nullptr) : Gate<width>(input) {
				// Empty
			}

			// SynchronousComponent interface
			void tick() {
				// Mathematical definition : only true if only ONE bit is set (first part)
				// Practical : also true if all bits are set (whole)
				this->out = (this->in->count() == 1) | this->in->all();
			}
	};

}

#endif // XORGATE_H
