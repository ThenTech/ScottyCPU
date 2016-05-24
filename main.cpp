/*
 *	Main file for executable.
 */

//#define	NDEBUG			// "No Debug" : comment to enable debugging

#include <iostream>

#include "Gate.h"

#ifndef NDEBUG
#include "UnitTest.h"
#endif

using namespace std;
using namespace CPUComponents;

/*
 *	Run main program.
 */
int main(int argc, char *argv[]) {

#ifndef NDEBUG
	runTests();
#else
	bitsetExample();
#endif

	return 0;
}
