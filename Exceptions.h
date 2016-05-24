#ifndef EXCEPTIONS
#define EXCEPTIONS

#include <stdexcept>

using namespace std;

namespace Exceptions {

	// logic_error	domain_error	invalid_argument
	// length_error	out_of_range	runtime_error
	// range_error	overflow_error	underflow_error

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

	class OutOfBoundsException : public Exception  {
	public:
		OutOfBoundsException(int idx)
			: Exception("Index " + to_string(idx) + " was out of bounds.") {}

		const string getMessage() const /*_GLIBCXX_USE_NOEXCEPT*/ {
			return "OutOfBoundsException: " + Exception::getMessage();
		}
	};

	class NullPointerException : public Exception  {
	public:
		NullPointerException(string msg)
			: Exception("Reference " + msg + " not set to an object.") {}

		const string getMessage() const /*_GLIBCXX_USE_NOEXCEPT*/ {
			return "NullPointerException: " + Exception::getMessage();
		}
	};

	class FileReadException : public Exception  {
	public:
		FileReadException()
			: Exception("Cannot read from file.") {}

		const string getMessage() const /*_GLIBCXX_USE_NOEXCEPT*/ {
			return "FileReadException: " + Exception::getMessage();
		}
	};

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

