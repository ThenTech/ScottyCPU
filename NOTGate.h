#ifndef NOTGATE_H
#define NOTGATE_H

#include "Gate.h"

namespace CPUComponents {

		class NOTGate : public Gate<1u> {
		public:
			NOTGate(std::bitset<1u> *const input = nullptr) : Gate<1u>(input) {
				// Empty
			}

			// SynchronousComponent interface
			void tick() {
				this->out = !this->in[0]; // !this->in->test(0) or this->in->flip()
			}
	};

}

#endif // NOTGATE_H
