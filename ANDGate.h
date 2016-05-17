#ifndef ANDGATE_H
#define ANDGATE_H

#include "Gate.h"

namespace CPUComponents {

	template <size_t width>
	class ANDGate : public Gate<width> {
		public:
			ANDGate(std::bitset<width> *const input = nullptr) : Gate<width>(input) {
				// Empty
			}

			// SynchronousComponent interface
			void tick() {
				this->out = this->in->all();
			}
	};

}

#endif // ANDGATE_H
