#ifndef SIGNEDBITSET_HPP
#define SIGNEDBITSET_HPP
#include <ostream>
#include <bitset>
#include "Exceptions.hpp"

namespace std {

	/**
	 *	\brief	**SignedBitset** : Encapsulate bitset to provide extended functionallity with signs and operators.
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class SignedBitset : public std::bitset<bit_width> {
		public:

			/**
			 *	Default constructor
			 */
			SignedBitset() : std::bitset<bit_width>() {}


			/**
			 *	Constructor with default value.
			 */
			SignedBitset(long long __val) : std::bitset<bit_width>(__val) {}

			/**
			 *	Default destructor
			 */
			~SignedBitset() {}

			/**	\brief	Get the sign of the SignedBitset.
			 *
			 *	\return	int
			 *      Returns the sign: -1 for negative, 0 for zero or 1 for positive.
			 */
			int sign() const {
				//return this->to_llong() < 0 ? -1 : this->to_llong() == 0 ? 0 : 1;
				return this->test(bit_width - 1) ? -1 : this->none() ? 0 : 1;
			}

			/**	\brief	Return the value of the bitset cast to long (with proper sign).
			 */
			long to_long(void) const {
				struct {long x:bit_width;} s;
				return s.x = this->to_ulong();
			}

			/**	\brief	Return the value of the bitset cast to long long (with proper sign).
			 */
			long long to_llong(void) const {
				struct {long long x:bit_width;} s;
				return s.x = this->to_ullong();
			}

			/**	\brief	Operator+ : Add 2 SignedBitsets and return a new SignedBitsets.
			 *
			 *	\param	__rhs
			 *      The bitset to add to this bitset.
			 *
			 *	\return	SignedBitset<bit_width>
			 *      A new SignedBitset with the sum.
			 */
			SignedBitset<bit_width> operator+(const SignedBitset<bit_width>& __rhs) const {
//				std::cout << "addition: " << this->to_llong() << " + "
//									   << __rhs.to_llong() << " = "
//									   << (this->to_llong() + __rhs.to_llong()) << std::endl;
				return SignedBitset<bit_width>(this->to_llong() + __rhs.to_llong());
			}

			/**	\brief	Operator+= : Add given SignedBitsets to this one.
			 *
			 *	\param	__rhs
			 *      The bitset to add to this bitset.
			 *
			 *	\return	SignedBitset<bit_width>
			 *      This bitset.
			 */
			SignedBitset<bit_width>& operator+=(const SignedBitset<bit_width>& __rhs) {
				return *this = *this + __rhs;
			}

			/**	\brief	Operator- : Negate this bitset.
			 *
			 *	\return	SignedBitset<bit_width>
			 *      A new SignedBitset with the negated value of this bitset.
			 */
			SignedBitset<bit_width> operator-(void) const {
				return SignedBitset<bit_width>(-this->to_llong());
			}

			/**	\brief	Operator- : Subtract given SignedBitsets from this one by making the sum of the negation.
			 *
			 *	\param	__rhs
			 *      The bitset to subtract from this bitset.
			 *
			 *	\return	SignedBitset<bit_width>
			 *      A new SignedBitset with the subtraction.
			 */
			SignedBitset<bit_width> operator-(const SignedBitset<bit_width>& __rhs) const {
				return -(-*this + __rhs);
			}

			/**	\brief	Operator-= : Subtract given SignedBitsets from this one.
			 *
			 *	\param	__rhs
			 *      The bitset to subtract from this bitset.
			 *
			 *	\return	SignedBitset<bit_width>
			 *      This bitset.
			 */
			SignedBitset<bit_width>& operator-=(const SignedBitset<bit_width>& __rhs) {
				return *this = *this - __rhs;
			}

			/**	\brief	Operator* : Multiply given SignedBitsets with this one.
			 *
			 *	\param	__rhs
			 *      The bitset to multiply with this bitset.
			 *
			 *	\return	SignedBitset<bit_width>
			 *      A new SignedBitset with the multiplication.
			 */
			SignedBitset<bit_width> operator*(const SignedBitset<bit_width>& __rhs) const {
				return SignedBitset<bit_width>(this->to_llong() * __rhs.to_llong());
			}

			/**	\brief	Operator*= : Multiply given SignedBitsets with this one.
			 *
			 *	\param	__rhs
			 *      The bitset to multiply with this bitset.
			 *
			 *	\return	SignedBitset<bit_width>
			 *      This bitset.
			 */
			SignedBitset<bit_width>& operator*=(const SignedBitset<bit_width>& __rhs) {
				return *this = *this * __rhs;
			}

			/**	\brief	Operator/ : Divide this SignedBitsets by the given one (no floating point deivision).
			 *
			 *	\param	__rhs
			 *      The bitset to divide this bitset by.
			 *
			 *	\return	SignedBitset<bit_width>
			 *      A new SignedBitset with the division.
			 *
			 *	\exception	Exceptions::DivideByZeroException
			 *		Throws exception if second bitset is zero (undefined behaviour).
			 */
			SignedBitset<bit_width> operator/(const SignedBitset<bit_width>& __rhs) const {
				#ifdef THROW_EXCEPTIONS
					if (__rhs.none())
						throw Exceptions::DivideByZeroException("SignedBitset");
				#endif
				return SignedBitset<bit_width>(this->to_llong() / __rhs.to_llong());
			}

			/**	\brief	Operator/= : Divide this SignedBitsets by the given one (no floating point deivision).
			 *
			 *	\param	__rhs
			 *      The bitset to divide this bitset by.
			 *
			 *	\return	SignedBitset<bit_width>
			 *      This bitset.
			 *
			 *	\exception	Exceptions::DivideByZeroException
			 *		Throws exception if second bitset is zero (undefined behaviour).
			 */
			SignedBitset<bit_width>& operator/=(const SignedBitset<bit_width>& __rhs) {
				return *this = *this / __rhs;
			}

			/**	\brief	Operator% : Get the Modulo from dividing this SignedBitsets by the given one.
			 *
			 *	\param	__rhs
			 *      The bitset to divide this bitset by.
			 *
			 *	\return	SignedBitset<bit_width>
			 *      A new SignedBitset with the division.
			 *
			 *	\exception	Exceptions::DivideByZeroException
			 *		Throws exception if second bitset is zero (undefined behaviour).
			 */
			SignedBitset<bit_width> operator%(const SignedBitset<bit_width>& __rhs) const {
				#ifdef THROW_EXCEPTIONS
					if (__rhs.none())
						throw Exceptions::DivideByZeroException("SignedBitset");
				#endif
				return SignedBitset<bit_width>(this->to_llong() % __rhs.to_llong());
			}

			/**	\brief	Operator% : Get the Modulo from dividing this SignedBitsets by the given one.
			 *
			 *	\param	__rhs
			 *      The bitset to divide this bitset by.
			 *
			 *	\return	SignedBitset<bit_width>
			 *      This bitset.
			 *
			 *	\exception	Exceptions::DivideByZeroException
			 *		Throws exception if second bitset is zero (undefined behaviour).
			 */
			SignedBitset<bit_width>& operator%=(const SignedBitset<bit_width>& __rhs) {
				return *this = *this % __rhs;
			}

			/**	\brief	Compare this bitset to the given one.
			 *
			 *	\param	__rhs
			 *      The bitset to compare to.
			 *
			 *	\return	SignedBitset<bit_width>
			 *      The result as given by the sign of this subtracted by the given bitset.
			 */
			SignedBitset<bit_width> compareTo(const SignedBitset<bit_width>& __rhs) const {
				return SignedBitset<bit_width>((*this - __rhs).sign());
			}

			/**	\brief	Add the signed value to a given stream (`os << SignedBitset`).
			 */
			friend std::ostream& operator<<(std::ostream &os, SignedBitset &s) {
				os << s.to_string() << " ==> " << s.to_llong();
				return os;
			}

			/**	\brief	Add the signed value to a given stream (`os << SignedBitset`).
			 */
			friend std::ostream& operator<<(std::ostream &os, SignedBitset *s) {
				return os << *s;
			}
	};

}

#endif // SIGNEDBITSET_HPP
