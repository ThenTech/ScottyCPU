/*
 *	Main file for executable.
 */

//#define	NDEBUG			// "No Debug" : comment to enable debugging

#include <bitset>
#include <iostream>

#include "Gate.h"

#ifndef NDEBUG
#include "UnitTest.h"
#endif

using namespace std;
using namespace CPUComponents;

void bitsetExample() {
	// Needs fixed size at compile time.
	// Alternative: vector<bool> ?
	bitset<5> y(8);

	cout << "x             : " << y				<< endl;
	cout << "As ulong      : " << y.to_ulong()	<< endl;
	cout << "As string     : " << y.to_string() << endl;
	cout << "x |= 16       = " << (y |= 16)		<< endl;
	cout << "x invert      = " << y.flip()		<< endl;
	cout << "x count       = " << y.count()		<< endl;
	cout << "x any (| all) = " << y.any()		<< endl; // OR
	cout << "x all (& all) = " << y.all()		<< endl; // AND
	cout << "x none(~|all) = " << y.none()		<< endl; // NOR
	cout << "x test.0      = " << y.test(0)		<< endl;
	cout << "x test.4      = " << y.test(4)		<< endl;
	cout << "x size        = " << y.size()		<< endl;

	cout << "x reset       = " << y.reset()		<< endl;
	cout << "x count       = " << y.count()		<< endl;
}

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
