#ifndef NORGATE_H
#define NORGATE_H

#include "Gate.h"

namespace CPUComponents {

	template <size_t width>
	class NORGate : public Gate<width> {
		public:
			NORGate(std::bitset<width> *const input = nullptr) : Gate<width>(input) {
				// Empty
			}

			// SynchronousComponent interface
			void tick() {
				this->out = this->in->none();
			}
	};

}

#endif // NORGATE_H
