#ifndef ORGATE_H
#define ORGATE_H

#include "Gate.h"

namespace CPUComponents {

	template <size_t width>
	class ORGate : public Gate<width> {
		public:
			ORGate(std::bitset<width> *const input = nullptr) : Gate<width>(input) {
				// Empty
			}

			// SynchronousComponent interface
			void tick() {
				this->out = this->in->any();
			}
	};

}

#endif // ORGATE_H
