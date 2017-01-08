/**
 *	Main file for ScottyCPU executable.
 */

//#define	NDEBUG			// "No Debug" : comment to enable debugging
#define	THROW_EXCEPTIONS	// Uncomment to enable Exception checking and throwing.

#include <iostream>
#include <iomanip>
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
	string version		= "0.4.42";
	bool parseScAM		= true;
	bool loadScHex		= false;
	string scamFile		= "";
	string schexFile	= "";
} ScottySettings;

/**
 *	\brief	Print the usage (command line arguments) of this program and exit.
 */
void showUsage(char* _name) {
	string name(_name),
		   help_1 = " [-h|-H] [-d|-D] [-c|-C <float>] [-i|-I]",
		   help_2 = " [-l|-L|-a|-A <file>] [-o <file>] [-hex <file>]";
	stringstream usage;

	name = "Usage: " + name.substr(name.find_last_of('\\') + 1);

	usage << name << help_1 << endl
		  << std::setw(name.size() + help_2.size()) << help_2				<< endl << endl
		  << "  -h, -H, --help     Show this help message"					<< endl
		  << "  -d, -D, --debug    Execute UnitTests"						<< endl
		  << "  -c, -C  <float>    Set the ScottyCPU clock frequency"		<< endl
		  << "  -i, -I             Show InstructionSet"						<< endl
		  << "  -l, -L  <file>     Load .ScAM file and parse"				<< endl
		  << "  -a, -A  <file>     Load .ScAM file and compile to .ScHex"	<< endl
		  << "  -o      <file>     Specify output file for assembly"		<< endl
		  << "  -hex    <file>     Load .ScHex file into ScottyCPU RAM"		<< endl
		  << endl;

	//fprintf(stderr, usage.str().c_str());
	cerr << usage.str();
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
			} else if (arg == "-h" || arg == "-H" || arg == "--help") {
				/// Show usage
				showUsage(argv[0]);
				SysUtils::callSystemCmd("PAUSE");
				exit(1);
			} else if (arg == "-c" || arg == "-C") {
				/// Set clk_freq
				ScottySettings.clk_freq = SysUtils::lexical_cast<float>(argv[++i]);
			} else if (arg == "-i" || arg == "-I") {
				/// Show InstructionSet
				CPUInstructions::printInstructionSet();
			} else if (arg == "-l" || arg == "-L" || arg == "-a" || arg == "-A") {
				/// Parse ScAM file
				ScottySettings.parseScAM = (arg == "-l" || arg == "-L");
				ScottySettings.scamFile = std::string(argv[++i]);
			} else if (arg == "-o") {
				/// Set output .ScHex file
				ScottySettings.schexFile = std::string(argv[++i]);
			} else if (arg == "-hex") {
				/// Load .ScHex file
				ScottySettings.loadScHex = true;
				ScottySettings.schexFile = std::string(argv[++i]);
			} else {
				/// Show usage
				showUsage(argv[0]);
				exit(1);
			}
		}

		SysUtils::setTitle(ScottySettings.version, ScottySettings.debug);

		#ifndef NDEBUG
		if (ScottySettings.debug)
			runTests();
		#endif

		if (!ScottySettings.scamFile.empty()) {
			if (ScottySettings.parseScAM) {
				CPUFactory::SCAMParser parser (ScottySettings.scamFile);
				std::cout << parser;
			} else {
				CPUFactory::SCAMAssembler assembler(ScottySettings.scamFile);
				std::cout << assembler;
				if (!assembler.hasErrors())
					assembler.exportScHex(ScottySettings.schexFile);
			}
		}

		if (ScottySettings.loadScHex) {
			CPUComponents::ScottyCPU<16u, 64u, 16u> cpu(ScottySettings.clk_freq);

			std::vector<char> *buffer = SysUtils::readBinaryFile(ScottySettings.schexFile);

			cpu.staticLoader(buffer);

			std::cout << "Loaded binary file \"" << ScottySettings.schexFile << "\" into RAM. " << std::endl;

			SysUtils::deallocVar(buffer);

			while (1) {
				cpu.step();
				SysUtils::callSystemCmd("PAUSE");
			}
		}

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
