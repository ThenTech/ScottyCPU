#ifndef UTILS_HPP
#define UTILS_HPP

#ifndef UNICODE
#define UNICODE
#endif

#include <iostream>
#include <sstream>
#include <typeinfo>
#ifndef _MSC_VER
	#include <cxxabi.h>
#endif
#include <limits>
#include <cmath>
#ifdef __WIN32__
	#include <windows.h>
#endif

#include "Exceptions.hpp"

#ifndef __cplusplus
#error A C++ compiler is required!
#endif

namespace std {

	/**	\brief	Replace all occurences of from with to in the given std::string str.
	 *
	 *	\param	str
	 *		A reference to the string to replace a substring.
	 *	\param	from
	 *		The characters to replace.
	 *	\param	to
	 *		The characters to replace with.
	 */
	static inline void strReplaceAll(string &str, const string& from, const string& to) {
		size_t start_pos = 0;
		while((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}
	}

	/**	\brief	Returns the internal actual class name of the given object o.
	 *
	 *	**Uses __abi::__cxa_demangle__ which is part of <cxxabi.h> included in all GCC compilers.**
	 *
	 *	If GCC is not used, type2name will revert to typeid(o).name() instead.
	 *
	 *	\param	T
	 *		The type of object.
	 *	\param	o
	 *		The object to demangle the name from.
	 *	\return
	 *		Returns the class name of o.
	 */
	template <class T>
	static inline const string type2name(T const& o) {
		#ifdef _CXXABI_H
				string s = string(abi::__cxa_demangle(typeid(o).name(),nullptr,nullptr,nullptr));
		#else
				string s = string(typeid(o).name());
		#endif

		std::strReplaceAll(s, string("std::"), string(""));           // Remove std:: from output
		std::strReplaceAll(s, string("CPUComponents::"), string("")); // Remove CPUComponents:: from output
		return s;
	}

}

namespace SysUtils {

	/**	\brief	Convert the given char* to a wchar_t* variable.
	 *
	 *	\param	buffer
	 *		The character buffer to convert.
	 *	\return
	 *		Returns L##buffer : each char is replaced with its wide version.
	 */
	inline wchar_t* convert2WSTR(const char* buffer) {
		wchar_t* WSTRbuff = new wchar_t[strlen(buffer) * 2 + 2];
		swprintf(WSTRbuff, (strlen(buffer) * 2 + 2), L"%s", buffer);
		return WSTRbuff;
	}

	/**	\brief
	 *	Convert the given char* to a variable of type T.
	 *	Use this method instead of the raw C functions: atoi, atof, atol, atoll.
	 *
	 *	\param	buffer
	 *		The character buffer to convert.
	 *	\return
	 *		Returns a variable of type T with the value as given in buffer.
	 */
	template <class T>
	inline T lexical_cast(const char* buffer) {
		T out;
		std::istringstream cast(buffer);
		if (!(cast >> out))
			throw Exceptions::CastingException(buffer, std::type2name(out));
		return out;
	}

	/**	\brief	Check if the given doubles are equal.
	 *
	 *	\param	x
	 *		The first value to compare.
	 *	\param	y
	 *		The second value to compare.
	 *	\param	epsilon
	 *		The precision to compare with (standard deviation of 1e-4 or 0.0001).
	 *
	 *	\return	bool
	 *		Returns whether x equals y within the given epsilon precision.
	 */
	inline bool epsilon_equals(double x, double y, double epsilon = 1e-4) {
		return std::abs(x - y) < epsilon;
	}

	/**	\brief	Call a system() cmd command.
	 *
	 *	\param	command
	 *		The command as string to call in the console window.
	 */
	inline void callSystemCmd(std::string command) {
		system(command.c_str());
	}

	/**	\brief	Sets the console window title to text.
	 *
	 *	Uses SetConsoleTitle() if available, else appends "TITLE" to the front and
	 *	reverts to SysUtils::callSystemCmd().
	 *
	 *	\param	text
	 *		The text to change the title to.
	 */
	void setWindowTitle(std::string text) {
		#ifdef _MSC_VER
			std::stringstream s;
			s << "TITLE " << text;
			callSystemCmd(s.str());
		#else
			SetConsoleTitle(convert2WSTR(text.c_str()));
		#endif // _MSC_VER
	}

	/**	\brief	Update the console window title with the ScottyCPU info and build stats.
	 *
	 *	\param	version
	 *		The current version of the program.
	 *	\param	debug
	 *		Whether this is a debug build.
	 */
	void setTitle(std::string version, bool debug) {
		std::stringstream s;
		s << "ScottyCPU  ::  v" << version << (debug ? "d" : "")
								<< "  ::  built on " << __DATE__ << " at " << __TIME__;
		SysUtils::setWindowTitle(s.str());
	}

	/*
	 *	Overloaded methods to allocate an array of T of size x, y, z.
	 */
	/**	\brief	Allocate an object of type T on the heap using `new T()`.
	 *
	 *	\param	T
	 *		The type of object to allocate.
	 *	\return
	 *		A pointer to the newly allocated object.
	 */
	template <class T>
	static inline T* allocVar() {
		return new T();
	}

	/**	\brief	Allocate an array of objects of type T and length x on the heap using `new T[x]()`.
	 *
	 *	\param	T
	 *		The type of object to allocate.
	 *	\param	x
	 *		The length of the array in the first dimension.
	 *	\return
	 *		A pointer to the newly allocated object.
	 */
	template <class T>
	static inline T* allocArray(size_t x) {
		return new T[x]();
	}

	/**	\brief	Allocate y arrays of objects of type T and length x on the heap.
	 *
	 *	\param	T
	 *		The type of object to allocate.
	 *	\param	x
	 *		The length of the array in the first dimension.
	 *	\param	y
	 *		The length of the array in the second dimension.
	 *	\return
	 *		A pointer to the newly allocated object.
	 */
	template <class T>
	static inline T** allocArray(size_t x, size_t y) {
		T **arr = new T*[x];
		for(size_t i = 0; i < x; i++) arr[i] = SysUtils::allocArray<T>(y);
		return arr;
	}

	/**	\brief	Allocate z arrays of y arrays of objects of type T and length x on the heap.
	 *
	 *	\param	T
	 *		The type of object to allocate.
	 *	\param	x
	 *		The length of the array in the first dimension.
	 *	\param	y
	 *		The length of the array in the second dimension.
	 *	\param	z
	 *		The length of the array in the third dimension.
	 *	\return
	 *		A pointer to the newly allocated object.
	 */
	template <class T>
	static inline T*** allocArray(size_t x, size_t y, size_t z) {
		T ***arr = new T**[x];
		for(size_t i = 0; i < x; i++) arr[i] = SysUtils::allocArray<T>(y, z);
		return arr;
	}
}

#endif // UTILS_HPP
