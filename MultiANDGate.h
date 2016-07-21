#ifndef MULTIANDGATE_H
#define MULTIANDGATE_H

#include "MultiGate.h"

namespace CPUComponents {

	template<size_t sets, size_t width>
	class MultiANDGate : public MultiGate<sets, width> {
		private:

		public:
			MultiANDGate(std::bitset<width> *const input) : MultiGate<sets, width>(input) {
				// Empty
			}

			MultiANDGate(std::bitset<width> *const *input = nullptr) : MultiGate<sets, width>(input) {
				// Empty
			}

			// SynchronousComponent interface
			void tick() {
				if (sets == 1)
					this->out = this->in->all();
				else {
					this->multi_out = *this->multi_in.front();

					for (auto& inbit : this->multi_in) {
						this->multi_out &= *inbit;
					}

					if (width == 1)
						this->out = this->multi_out.test(0);
				}
			}
	};

}

#endif // MULTIANDGATE_H
