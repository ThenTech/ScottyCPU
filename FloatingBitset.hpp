#ifndef FLOATINGBITSET_HPP
#define FLOATINGBITSET_HPP
#include <ostream>
#include <bitset>
#include "SignedBitset.hpp"
#include "Exceptions.hpp"

namespace std {

	/**
	 *	\brief	**FloatingBitset** : Encapsulate bitset to provide extended functionallity with signs and operators and floating point math.
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class FloatingBitset : public SignedBitset<bit_width> {
		private:

			/**
			 *	\brief	Extra bitset variable containing a fractionised floating point number.
			 *
			 *		From left to right, each bit corresponds with the fraction: 1 / (current bit)
			 *
			 *		MSB = 1 / 2, ..., LSB = 1 / (2 << bit_width)
			 *
			 *		e.g.: 0.75 = 0.5 + 0.25 ==> (1100) in 4 bits
			 */
			std::bitset<bit_width> digits;

		public:

			/**
			 *	Default constructor
			 */
			FloatingBitset() : std::bitset<bit_width>() {}

			/**
			 *	\brief	Constructor with default value for number before comma.
			 */
			FloatingBitset(unsigned long long __val) : SignedBitset<bit_width>(__val), digits(0) {}

			/**
			 *	\brief	Constructor with default value.
			 *		Value before comma is the given double floored.
			 *		Value after comma is calculated by subtracting fractions (1 / factor) from the remaining number.
			 */
			FloatingBitset(double __val) : SignedBitset<bit_width>((unsigned long long) __val) {
				__val -= (unsigned long long) __val;
				float q = 0.0;

				for (int i = bit_width, factor = 2; --i >= 0; factor <<= 1) {
					q = (1.0F / factor);
					if (__val >= q) {
						__val -= q;
						digits.set(i);
					}
				}
			}

			/**
			 *	Default destructor
			 */
			~FloatingBitset() {}

			/**	\brief	Return the value of the bitset cast to float (with proper sign and digits behind comma).
			 */
			float to_float(void) const {
				return (float) this->to_double();
			}

			/**	\brief	Return the value of the bitset cast to double (with proper sign and digits behind comma).
			 *		Calculated by adding the fractions given in `this->digits`.
			 */
			double to_double(void) const {
				double result = 0.0F;

				for (int i = bit_width, factor = 2; --i >= 0; factor <<= 1) {
					result += this->digits.test(i) ? 1.0F / factor : 0.0F;
				}

				return (double)this->to_llong() + result;
			}

			/**	\brief	Return the digits behind the comma of the FloatingBitset.
			 */
			const std::bitset<bit_width>& getDigits(void) const {
				return this->digits;
			}

			/**	\brief	Operator* : Multiply given FloatingBitset with this one.
			 *
			 *	\param	__rhs
			 *      The bitset to multiply with this bitset.
			 *
			 *	\return	FloatingBitset<bit_width>
			 *      A new SignedBitset with the multiplication.
			 */
			FloatingBitset<bit_width> operator*(const FloatingBitset<bit_width>& __rhs) const {
				return FloatingBitset<bit_width>(this->to_double() * __rhs.to_double());
			}

			/**	\brief	Operator*= : Multiply given FloatingBitset with this one.
			 *
			 *	\param	__rhs
			 *      The bitset to multiply with this bitset.
			 *
			 *	\return	FloatingBitset<bit_width>
			 *      This bitset.
			 */
			FloatingBitset<bit_width>& operator*=(const FloatingBitset<bit_width>& __rhs) {
				return *this = *this * __rhs;
			}

			/**	\brief	Operator/ : Divide this FloatingBitset by the given one (floating point deivision).
			 *
			 *	\param	__rhs
			 *      The bitset to divide this bitset by.
			 *
			 *	\return	FloatingBitset<bit_width>
			 *      A new FloatingBitset with the division.
			 *
			 *	\exception	Exceptions::DivideByZeroException
			 *		Throws exception if second bitset is zero (undefined behaviour).
			 */
			FloatingBitset<bit_width> operator/(const FloatingBitset<bit_width>& __rhs) const {
				#ifdef THROW_EXCEPTIONS
					if (__rhs.none())
						throw Exceptions::DivideByZeroException("FloatingBitset");
				#endif
				return FloatingBitset<bit_width>(this->to_double() / __rhs.to_double());
			}

			/**	\brief	Operator/= : Divide this FloatingBitset by the given one (floating point deivision).
			 *
			 *	\param	__rhs
			 *      The bitset to divide this bitset by.
			 *
			 *	\return	FloatingBitset<bit_width>
			 *      This bitset.
			 *
			 *	\exception	Exceptions::DivideByZeroException
			 *		Throws exception if second bitset is zero (undefined behaviour).
			 */
			FloatingBitset<bit_width>& operator/=(const FloatingBitset<bit_width>& __rhs) {
				return *this = *this / __rhs;
			}

			/**	\brief	Add the floating point value to a given stream (`os << SignedBitset`).
			 */
			friend std::ostream& operator<<(std::ostream &os, FloatingBitset &s) {
				os.precision(10);
				os << s.to_double();
				//os << s.to_string() << " ==> " << s.to_double();
				return os;
			}

			/**	\brief	Add the floating point value to a given stream (`os << SignedBitset`).
			 */
			friend std::ostream& operator<<(std::ostream &os, FloatingBitset *s) {
				return os << *s;
			}

	};

}

#endif // FLOATINGBITSET_HPP
