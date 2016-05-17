#ifndef NANDGATE_H
#define NANDGATE_H

#include "Gate.h"

namespace CPUComponents {

	template <size_t width>
	class NANDGate : public Gate<width> {
		public:
			NANDGate(std::bitset<width> *const input = nullptr) : Gate<width>(input) {
				// Empty
			}

			// SynchronousComponent interface
			void tick() {
				this->out = !this->in->all();
			}
	};

}

#endif // NANDGATE_H
