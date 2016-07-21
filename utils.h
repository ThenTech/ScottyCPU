#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <typeinfo>
#include <cxxabi.h>
#include <limits>
#include <windows.h>

#ifndef __cplusplus
#error A C++ compiler is required!
#endif

namespace std {
	static inline void strReplaceAll(string &str, const string& from, const string& to) {
		size_t start_pos = 0;
		while((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}
	}

	template <class T>
	static inline const string type2name(T o) {
		string s = string(abi::__cxa_demangle(typeid(o).name(),nullptr,nullptr,nullptr));
		std::strReplaceAll(s, string("std::"), string(""));			// Remove std:: from output
		std::strReplaceAll(s, string("CPUComponents::"), string("")); // Remove CPUComponents:: from output
		return s;
	}
}

namespace SysUtils {

	inline wchar_t* convert2WSTR(const char* buffer) {
		wchar_t* WSTRbuff = new wchar_t[strlen(buffer) * 2 + 2];
		swprintf(WSTRbuff, L"%s", buffer);
		return WSTRbuff;
	}

	inline void setWindowTitle(std::string text) {
		#ifdef _INC_WINDOWS
			SetConsoleTitle(convert2WSTR(text.c_str()));
		#else
			system(text.c_str());
		#endif
	}

	inline void setTitle(float version, bool debug) {
		std::stringstream s;
		s << "ScottyCPU  ::  v" << version << (debug ? "d" : "") << "  ::  built on " << __DATE__ << " at " << __TIME__;
		SysUtils::setWindowTitle(s.str());
	}

	/**
	 *	Overloaded methods to allocate an array of T of size x, y, z.
	 */
	template <class T>
	static inline T* allocVar() {
		return new T();
	}

	template <class T>
	static inline T* allocArray(size_t x) {
		return new T[x]();
	}

	template <class T>
	static inline T** allocArray(size_t x, size_t y) {
		T **arr = new T*[x];
		for(size_t i = 0; i < x; i++) arr[i] = SysUtils::allocArray<T>(y);
		return arr;
	}

	template <class T>
	static inline T*** allocArray(size_t x, size_t y, size_t z) {
		T ***arr = new T**[x];
		for(size_t i = 0; i < x; i++) arr[i] = SysUtils::allocArray<T>(y, z);
		return arr;
	}
}

#endif // UTILS_H
