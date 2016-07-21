/*
 *	Main file for executable.
 */

//#define	NDEBUG			// "No Debug" : comment to enable debugging

#include <iostream>
#include <sstream>

#include "utils.h"
#include "Gate.h"

#ifndef NDEBUG
#include "UnitTest.h"
#endif

using namespace std;
using namespace CPUComponents;

static float scottyVersion = 0.1;

/*
 *	Run main program.
 */
int main(int argc, char *argv[]) {

#ifndef NDEBUG
	SysUtils::setTitle(scottyVersion, true);
	runTests();
#else
	// Normal Execution
	SysUtils::setTitle(scottyVersion, false);
#endif

	return 0;
}
