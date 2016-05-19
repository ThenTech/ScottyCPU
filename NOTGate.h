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
				// Or
				//	!this->in->test(0)
				// Or other Gate<2>s with same bit on both inputs
				//	NOR:  !this->in->any() or this->in->none()
				//	NAND: !this->in->all()
				this->out =  this->in->none();
			}
	};

}

#endif // NOTGATE_H
