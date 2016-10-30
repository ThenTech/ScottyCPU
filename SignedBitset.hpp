#ifndef SIGNEDBITSET_HPP
#define SIGNEDBITSET_HPP

#include <bitset>

namespace std {

	template <size_t bit_width>
	class SignedBitset : public std::bitset<bit_width> {
		public:

			SignedBitset() : std::bitset<bit_width>() {

			}

			SignedBitset(long long __val) : std::bitset<bit_width>(__val) {

			}

			int sign() const {
				return this->test(bit_width) ? -1 : this->none() ? 0 : 1;
			}

			long to_long(void) {
				struct {long x:bit_width;} s;
				return s.x = this->to_ulong();
			}

			long long to_llong(void) {
				struct {long long x:bit_width;} s;
				return s.x = this->to_ullong();
			}
	};

}

#endif // SIGNEDBITSET_HPP
