#ifndef EXCEPTIONS
#define EXCEPTIONS

#include <stdexcept>

//#define	THROW_EXCEPTIONS

using namespace std;

namespace Exceptions {

	// logic_error	domain_error	invalid_argument
	// length_error	out_of_range	runtime_error
	// range_error	overflow_error	underflow_error

	/**
	 *	\brief
	 *		Generic exception with message.
	 *	\param	msg
	 *		The message to be shown when encountering this exception.
	 */
	class Exception : public exception {
	private:
	   string _msg;
	public:
		explicit Exception(const string& msg) : _msg(msg) {}
		virtual ~Exception() /*_GLIBCXX_USE_NOEXCEPT*/ {}

		virtual const string getMessage() const /*_GLIBCXX_USE_NOEXCEPT*/ {
			return "Exception: " + _msg;
		}

		virtual const string getMsg() const /*_GLIBCXX_USE_NOEXCEPT*/ {
			return _msg;
		}
	};

	/**
	 *	\brief
	 *		Out of bounds exception with message and index.
	 *	\param	idx
	 *		The index that caused the out of bounds.
	 */
	class OutOfBoundsException : public Exception  {
	public:
		OutOfBoundsException(int idx)
			: Exception("Index " + to_string(idx) + " was out of bounds.") {}

		const string getMessage() const /*_GLIBCXX_USE_NOEXCEPT*/ {
			return "OutOfBoundsException: " + Exception::getMsg();
		}
	};

	/**
	 *	\brief
	 *		Nullpointer exception with message.
	 *	\param	msg
	 *		The null reference.
	 */
	class NullPointerException : public Exception  {
	public:
		NullPointerException(string msg)
			: Exception("Reference " + msg + " not set to an object.") {}

		const string getMessage() const /*_GLIBCXX_USE_NOEXCEPT*/ {
			return "NullPointerException: " + Exception::getMsg();
		}
	};

	/**
	 *	\brief
	 *		Divide by zero exception with message.
	 *	\param	msg
	 *		The index that caused the out of bounds.
	 */
	class DivideByZeroException : public Exception  {
	public:
		DivideByZeroException(string msg)
			: Exception("Tried to devide by zero in " + msg) {}

		const string getMessage() const /*_GLIBCXX_USE_NOEXCEPT*/ {
			return "DivideByZeroException: " + Exception::getMsg();
		}
	};

	/**
	 *	\brief
	 *		Casting exception with message.
	 *	\param	src
	 *		The object to be casted.
	 *	\param	dest
	 *		The object to cast to.
	 */
	class CastingException : public Exception  {
	public:
		CastingException(string src, string dest)
			: Exception("Cannot cast \"" + src + "\" to object of type \"" + dest + "\"!") {}

		const string getMessage() const /*_GLIBCXX_USE_NOEXCEPT*/ {
			return "CastingException: " + Exception::getMsg();
		}
	};

	/**
	 *	\brief
	 *		File read exception.
	 */
	class FileReadException : public Exception  {
	public:
		FileReadException(string name)
			: Exception("Cannot read from file: " + name) {}

		const string getMessage() const /*_GLIBCXX_USE_NOEXCEPT*/ {
			return "FileReadException: " + Exception::getMsg();
		}
	};

	/**
	 *	\brief
	 *		File write exception.
	 */
	class FileWriteException : public Exception  {
	public:
		FileWriteException()
			: Exception("Cannot write to file.") {}

		const string getMessage() const /*_GLIBCXX_USE_NOEXCEPT*/ {
			return "FileWriteException: " + Exception::getMsg();
		}
	};
	/**
	 *	\brief
	 *		Unexpected file extension exception.
	 */
	class UnexpectedExtension : public Exception  {
	public:
		UnexpectedExtension(string ext)
			: Exception(ext) {}

		const string getMessage() const /*_GLIBCXX_USE_NOEXCEPT*/ {
			return "UnexpectedExtension: " + Exception::getMsg();
		}
	};
}


#endif // EXCEPTIONS

