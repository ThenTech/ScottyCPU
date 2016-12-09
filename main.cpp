/**
 *	Main file for ScottyCPU executable.
 */

//#define	NDEBUG			// "No Debug" : comment to enable debugging
#define	THROW_EXCEPTIONS	// Uncomment to enable Exception checking and throwing.

#include <iostream>
#include "ScottyCPU.hpp"

#include "utils.hpp"

#ifndef NDEBUG
#include "UnitTest.hpp"
#endif

using namespace std;

/**	\brief	Global settings fetched from command line arguments.
 *
 *  \param	clk_freq
 *		The clock frequency.
 *  \param	debug
 *		Whether to execute UnitTests.
 *	\param	version
 *		The current version of this program.
 */
static struct SETTINGS {
	float clk_freq		= 1.0F;
	bool debug			= false;
	string version		= "0.4.18";
} ScottySettings;

/**
 *	\brief	Print the usage (command line arguments) of this program and exit.
 */
void showUsage(char* _name) {
	string name(_name);
	stringstream usage;
	usage << "Usage: " << name.substr(name.find_last_of('\\') + 1)
		  << " [-d|-D] [-c|-C <float>]" << endl
		  << "  -d, -D, --debug    Execute UnitTests" << endl
		  << "  -c, -C <float>     Set the ScottyCPU clock frequency" << endl << endl;

	//fprintf(stderr, usage.str().c_str());
	cerr << usage.str();
	exit(1);
}

/**
 *	Run main program.
 */
int main(int argc, char *argv[]) {
	int i;
	string arg;

	SysUtils::setTitle(ScottySettings.version, ScottySettings.debug);

	if (argc < 2) {
		showUsage(argv[0]);
		exit(1);
	}

	try {
		for (i = 1; i < argc; ++i) {
			arg = argv[i];

			if (arg == "-d" || arg == "-D" || arg == "--debug") {
				/// Enable debugging
				ScottySettings.debug = true;
			} else if (arg == "-h" || arg == "-H") {
				/// Show usage
				showUsage(argv[0]);
			} else if (arg == "-c" || arg == "-C") {
				/// Set clk_freq
				ScottySettings.clk_freq = SysUtils::lexical_cast<float>(argv[++i]);
			}
		}

		SysUtils::setTitle(ScottySettings.version, ScottySettings.debug);

		#ifndef NDEBUG
		if (ScottySettings.debug)
			runTests();
		#endif

		CPUComponents::ScottyCPU<16u, 10u, 10u> cpu(ScottySettings.clk_freq);

	} catch (Exceptions::Exception const& e) {
		std::cerr << e.getMessage() << std::endl;
	} catch(std::exception const& e) {
		std::cerr << "std::exception: " << e.what() << std::endl;
	} catch(...) {
		std::cerr << "Unknown exception." << std::endl;
	}

	SysUtils::callSystemCmd("PAUSE");
	return 0;
}
