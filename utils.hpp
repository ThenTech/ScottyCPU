#ifndef UTILS_HPP
#define UTILS_HPP

#ifndef UNICODE
#define UNICODE
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <cctype>
#include <locale>
#include <typeinfo>
#include <stdarg.h>
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

#define UINT(X)		((size_t) X)
#define BOOLSTR(X)	((X) ? "true" : "false")

namespace std {

	// trim from start (in place)
	static inline void ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(),
				std::not1(std::ptr_fun<int, int>(std::isspace))));
	}

	// trim from end (in place)
	static inline void rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(),
				std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	}

	// trim from both ends (in place)
	static inline void trim(std::string &s) {
		ltrim(s);
		rtrim(s);
	}

	// trim from both ends (copying)
	static inline std::string trimmed(std::string s) {
		trim(s);
		return s;
	}

	// erase from erase_from until end (in place)
	static inline void strEraseFrom(string &str, const string& erase_from) {
		str = str.substr(0, str.find(erase_from));
	}

	// erase from last occurence of erase_from until end (in place)
	static inline void strEraseFromLast(string &str, const string& erase_from) {
		str = str.substr(0, str.find_last_of(erase_from));
	}

	// erase from begin to erase_to (in place)
	static inline void strEraseTo(string &str, const string& erase_to) {
		str = str.substr(str.find(erase_to));
	}

	// erase from begin to the last occurence of erase_to (in place)
	static inline void strEraseToLast(string &str, const string& erase_to) {
		str = str.substr(str.find_last_of(erase_to));
	}

	// erase from erase_from until end (copying)
	static inline std::string strErasedFrom(string str, const string& erase_from) {
		strEraseFrom(str, erase_from);
		return str;
	}

	// erase from last occurence of erase_from until end (copying)
	static inline std::string strErasedFromLast(string str, const string& erase_from) {
		strEraseFromLast(str, erase_from);
		return str;
	}

	// erase from begin to erase_to (copying)
	static inline std::string strErasedTo(string str, const string& erase_to) {
		strEraseTo(str, erase_to);
		return str;
	}

	// erase from begin to the last occurence of erase_to (copying)
	static inline std::string strErasedToLast(string str, const string& erase_to) {
		strEraseToLast(str, erase_to);
		return str;
	}

	// toUpper (in place)
	static inline void strToUpper(string &str) {
		std::transform(str.begin(), str.end(), str.begin(),
			[](std::string::value_type ch) {
				return std::use_facet<std::ctype<std::string::value_type>>(std::locale()).toupper(ch);
			}
		);
	}

	// toUpper (copying)
	static inline std::string strToUppercase(string str) {
		strToUpper(str);
		return str;
	}

	// toLower (in place)
	static inline void strToLower(string &str) {
		std::transform(str.begin(), str.end(), str.begin(),
			[](std::string::value_type ch) {
				return std::use_facet<std::ctype<std::string::value_type>>(std::locale()).tolower(ch);
			}
		);
	}

	// toLower (copying)
	static inline std::string strToLowercase(string str) {
		strToLower(str);
		return str;
	}

	static inline bool strHasChar(const string &str, const char ch) {
		// string.h : strchr(str.c_str(), ch)
		return str.find(ch) != std::string::npos;
	}

	static inline void strReplaceConsecutive(string &str, const char ch) {
		str.erase(std::unique(str.begin(), str.end(),
								[&](const char lhs, const char rhs) {
									return (lhs == ch) && (lhs == rhs);
								}
							 ), str.end());
	}

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

	template <class key_type, class mapped_type>
	static inline bool mapHasKey(const std::unordered_map<key_type, mapped_type>& map, const string& key) {
		return map.find(key) != map.end();
	}

	template <class key_type, class mapped_type>
	static inline const mapped_type& mapGetMapped(const std::unordered_map<key_type, mapped_type>& map, const string& key) {
		//if (mapHasKey(map, key))
		//returnmap.find(key)->second;
		return map.at(key);
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
			char *demang = abi::__cxa_demangle(typeid(o).name(), nullptr, nullptr, nullptr);
			string s(demang);
			std::free(demang);
		#else
			string s(typeid(o).name());
		#endif

		std::strReplaceAll(s, "std::", "");           // Remove std:: from output
		std::strReplaceAll(s, "CPUComponents::", ""); // Remove CPUComponents:: from output
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
		wchar_t* WSTRbuff = new wchar_t[strlen(buffer) * 2 + 1];
		#ifdef _MSC_VER
			swprintf_s(WSTRbuff, (strlen(buffer) * 2 + 1), L"%hs", buffer);
		#else
			swprintf(WSTRbuff, (strlen(buffer) * 2 + 1), L"%hs", buffer);
		#endif
		return WSTRbuff;
	}

	inline std::string stringFormat(char* buffer, size_t length, const char* format, ...) {
		va_list args;

		va_start(args, format);
		#ifdef _MSC_VER
			vsprintf_s(buffer, length, format, args);
		#else
			(void) length;
			vsprintf(buffer, format, args);
		#endif
		va_end(args);

		return std::string(buffer);
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
		std::stringstream cast;

		if (strToUppercase(std::string(buffer)).substr(0, 2) == "0X")
				cast << std::hex << buffer;
		else	cast << buffer;

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
			wchar_t *buff = convert2WSTR(text.c_str());
			SetConsoleTitle(buff);
			delete[] buff;
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

	/**
	 *	\brief TO-DO
	 *	\param filename
	 *	\return
	 */
	const std::string* readStringFromFile(const std::string filename) {
		std::string *str = new std::string();
		std::fstream file(filename);

		try {
			file.seekg(0, std::ios::end);
			str->reserve((size_t) file.tellg());
			file.seekg(0, std::ios::beg);

			str->assign((std::istreambuf_iterator<char>(file)),
						 std::istreambuf_iterator<char>());
		} catch (...) {
			file.close();
			throw Exceptions::FileReadException(filename);
		}

		file.close();
		return str;
	}

	/**
	 *	\brief TO-DO
	 *	\param filename
	 *	\param str
	 */
	void writeStringToFile(const std::string filename, const std::string& str) {
		std::ofstream file(filename);

		try {			
			file << str;
		} catch (...) {
			file.close();
			throw Exceptions::FileWriteException(filename);
		}

		file.close();
	}

	/**
	 *	\brief TO-DO
	 *	\param filename
	 *
	 *	\return	std::vector<char>*
	 *			A vector with buffer->size() bytes containing a program in raw binary.
	 *			Print with cast to (unsigned char) for proper viewing.
	 */
	std::vector<char>* readBinaryFile(const std::string filename) {
		std::vector<char> *v_buff = new std::vector<char>();
		std::ifstream file(filename, std::ifstream::binary);

		try {
			file.seekg(0, std::ios::end);
			v_buff->reserve((size_t) file.tellg());
			file.seekg(0, std::ios::beg);
			v_buff->assign((std::istreambuf_iterator<char>(file)),
							std::istreambuf_iterator<char>());
		} catch (...) {
			file.close();
			throw Exceptions::FileReadException(filename);
		}

		file.close();
		return v_buff;
	}

	/**
	 *	\brief TO-DO
	 *	\param filename
	 *	\param str
	 */
	void writeBinaryFile(const std::string filename, const char* buffer, size_t length) {
		std::ofstream file(filename, std::ofstream::binary);

		try {
			file.write(buffer, length);
		} catch (...) {
			file.close();
			throw Exceptions::FileWriteException(filename);
		}

		file.close();
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

	template <class T>
	static inline void deallocVar(T* v) {
		delete v;
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

	template <class T>
	static inline void deallocArray(T* a) {
		delete[] a;
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

	template <class T>
	static inline void deallocArray(T** a, size_t y) {
		for(size_t i = 0; i < y; i++) SysUtils::deallocArray(a[i]);
		SysUtils::deallocArray(a);
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

	template <class T>
	static inline void deallocArray(T*** a, size_t y, size_t z) {
		for(size_t i = 0; i < z; i++) SysUtils::deallocArray(a[i], y);
		SysUtils::deallocArray(a);
	}

	template <class T>
	static inline void deallocVector(std::vector<T> *v) {
		for (T i : *v) SysUtils::deallocVar(i);
		SysUtils::deallocVar(v);
	}

	template <class FIRST, class SECOND>
	static inline void deallocMap(std::unordered_map<FIRST, SECOND> *m) {
		for (std::pair<FIRST, SECOND> i : *m) SysUtils::deallocVar(i.second);
		SysUtils::deallocVar(m);
	}
}

#endif // UTILS_HPP
