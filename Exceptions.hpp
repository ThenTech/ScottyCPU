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
			return "OutOfBoundsException: " + Exception::getMessage();
		}
	};

	/**
	 *	\brief
	 *		Nullpointer exception with message.
	 *	\param	msg
	 *		The index that caused the out of bounds.
	 */
	class NullPointerException : public Exception  {
	public:
		NullPointerException(string msg)
			: Exception("Reference " + msg + " not set to an object.") {}

		const string getMessage() const /*_GLIBCXX_USE_NOEXCEPT*/ {
			return "NullPointerException: " + Exception::getMessage();
		}
	};

	/**
	 *	\brief
	 *		File read exception.
	 */
	class FileReadException : public Exception  {
	public:
		FileReadException()
			: Exception("Cannot read from file.") {}

		const string getMessage() const /*_GLIBCXX_USE_NOEXCEPT*/ {
			return "FileReadException: " + Exception::getMessage();
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
			return "FileWriteException: " + Exception::getMessage();
		}
	};
}


#endif // EXCEPTIONS

