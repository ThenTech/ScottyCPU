#ifndef UNITTEST_H
#define UNITTEST_H

#ifndef NDEBUG		// ifndef NDEBUG

#include <iostream>
#include <cassert>	// Debug assertion
#include "Gate.h"
#include "ANDGate.h"

using namespace CPUComponents;

/*
 *	Constant bit values.
 */
static std::bitset<1>	one_bit_0 = 0,	// 0
						one_bit_1 = 1;	// 1
static std::bitset<2>	two_bit_0 = 0,	// 00
						two_bit_1 = 1,	// 01
						two_bit_2 = 2,	// 10
						two_bit_3 = 3;	// 11
static std::bitset<3>	thr_bit_0 = 0,	// 000
						thr_bit_1 = 1,	// 001
						thr_bit_2 = 2,	// 010
						thr_bit_3 = 3,	// 011
						thr_bit_4 = 4,	// 100
						thr_bit_5 = 5,	// 101
						thr_bit_6 = 6,	// 110
						thr_bit_7 = 7;	// 111

/*
 *	Gate : Test basic Gate class.
 */
void testGate(void) {
	std::bitset<2> two_bits = 0;

	Gate<2>	g_with_null,
			g_with_1(&two_bits),
			*g_pointed = new Gate<2>(&two_bits);

	assert(g_pointed != nullptr);

	assert(g_with_null.output().size()	== 1);
	assert(g_with_null.output()			== 0);
	assert(g_with_1.output().size()		== 1);
	assert(g_with_1.output()			== 0);
	assert(g_pointed->output().size()	== 1);
	assert(g_pointed->output()			== 0);
	two_bits = 3;
	assert(g_with_null.output()			== 0);
	assert(g_with_1.output()			== 0);
	assert(g_pointed->output()			== 0);

	delete g_pointed;
	assert(two_bits == 3);	// Source not deleted
}

/*
 *	AND Gate : Test basic logic.
 */
void testLogic_AND_const(void) {
	ANDGate<2>	g2_0(&two_bit_0),
				g2_1(&two_bit_1),
				g2_2(&two_bit_2),
				g2_3(&two_bit_3);
	ANDGate<3>	g3_0(&thr_bit_0),
				g3_1(&thr_bit_1),
				g3_2(&thr_bit_2),
				g3_3(&thr_bit_3),
				g3_4(&thr_bit_4),
				g3_5(&thr_bit_5),
				g3_6(&thr_bit_6),
				g3_7(&thr_bit_7);

	//AND Gate with width 2 (2 inputs)
	assert(g2_0.output()			== 0);
	g2_0.tick();
	assert(g2_0.output()			== 0);

	assert(g2_1.output()			== 0);
	g2_1.tick();
	assert(g2_1.output()			== 0);

	assert(g2_2.output()			== 0);
	g2_2.tick();
	assert(g2_2.output()			== 0);

	assert(g2_3.output()			== 0);
	g2_3.tick();
	assert(g2_3.output()			== 1);	// Output changed to 1 after tick()

	//AND Gate with width 2 (2 inputs)
	assert(g3_0.output()			== 0);
	g3_0.tick();
	assert(g3_0.output()			== 0);

	assert(g3_1.output()			== 0);
	g3_1.tick();
	assert(g3_1.output()			== 0);

	assert(g3_2.output()			== 0);
	g3_2.tick();
	assert(g3_2.output()			== 0);

	assert(g3_3.output()			== 0);
	g3_3.tick();
	assert(g3_3.output()			== 0);

	assert(g3_4.output()			== 0);
	g3_4.tick();
	assert(g3_4.output()			== 0);

	assert(g3_5.output()			== 0);
	g3_5.tick();
	assert(g3_5.output()			== 0);

	assert(g3_6.output()			== 0);
	g3_6.tick();
	assert(g3_6.output()			== 0);

	assert(g3_7.output()			== 0);
	g3_7.tick();
	assert(g3_7.output()			== 1);	// Output changed to 1 after tick()
}

/*
 *	AND Gate : Test basic logic.
 */
void testLogic_AND_dynamic(void) {
	int i;
	std::bitset<2> two_bits = 0;
	std::bitset<3> thr_bits = 0;

	ANDGate<2>	g2(&two_bits);
	ANDGate<3>	g3(&thr_bits);

	two_bits = two_bit_3;
	assert(g2.output()		== 0);
	g2.tick();
	assert(g2.output()		== 1);

	thr_bits = thr_bit_7;
	assert(g3.output()		== 0);
	g3.tick();
	assert(g3.output()		== 1);

	for (two_bits = i = 0; i < 4; two_bits = ++i) {
		g2.tick();											// Update output
		assert(g2.output()	== (two_bits == two_bit_3));	// Output change to 1 if two_bits == 11
	}

	for (thr_bits = i = 0; i < 8; thr_bits = ++i) {
		g3.tick();											// Update output
		assert(g3.output()	== (thr_bits == thr_bit_7));	// Output change to 1 if thr_bits == 111
	}
}

/*
 *	Run all tests.
 */
void runTests(void) {
	testGate();
	testLogic_AND_const();
	testLogic_AND_dynamic();

}


#endif // ifndef NDEBUG

#endif // UNITTEST_H
