#ifndef UNITTEST_H
#define UNITTEST_H

#ifndef NDEBUG		// ifndef NDEBUG

#include <iostream>
#include <cassert>	// Debug assertion
#include "Exceptions.h"

#include "Gate.h"

#include "MultiGate.h"	//?
#include "MultiANDGate.h"

#include "ANDGate.h"
#include "NANDGate.h"
#include "ORGate.h"
#include "NORGate.h"
#include "XORGate.h"
#include "NOTGate.h"

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
static std::bitset<4>	for_bit_0 = 0,	// 0000
						for_bit_1 = 1,	// 0001
						for_bit_8 = 8,	// 1000
						for_bit_A = 10,	// 1010
						for_bit_F = 15;	// 1111

/*
 *	Bitset : Test basic bitset methods to show how they work.
 */
void testBitset(void) {
	// Needs fixed size at compile time.
	std::bitset<4> x = 0xA, y;

	assert(x == for_bit_A);
	std::cout << "x             : " << x				<< std::endl;

	assert(x.to_ulong() == 10ul);
	std::cout << "As ulong      : " << x.to_ulong()		<< std::endl;

	assert(x.to_string() == "1010");
	std::cout << "As string     : " << x.to_string()	<< std::endl;

	std::cout << "x |= 4        = " << (x |= 4)			<< std::endl;
	assert(x == 0xE);

	std::cout << "x invert      = " << x.flip()			<< std::endl;
	assert(x == for_bit_1);

	std::cout << "x count       = " << x.count()		<< std::endl;
	assert(x.count() == 1);

	std::cout << "x any (| all) = " << x.any()			<< std::endl; // OR
	assert(x.any() == true);

	std::cout << "x all (& all) = " << x.all()			<< std::endl; // AND
	assert(x.all() == false);

	std::cout << "x none(~|all) = " << x.none()			<< std::endl; // NOR
	assert(x.all() == false);

	std::cout << "x test.0      = " << x.test(0)		<< std::endl;
	assert(x.test(0) == true);

	std::cout << "x test.3      = " << x.test(3)		<< std::endl;
	assert(x.test(3) == false);

	std::cout << "x size        = " << x.size()			<< std::endl;
	assert(x.size() == 4);

	std::cout << "x reset       = " << x.reset()		<< std::endl;
	assert(x == for_bit_0);

	std::cout << "x count       = " << x.count()		<< std::endl;
	assert(x.count() == 0);

	std::cout << "x set         = " << x.set()			<< std::endl;
	assert(x == for_bit_F);

	std::cout << "x set.3=0     = " << x.set(3, false)	<< std::endl;
	assert(x == 0x7);

	std::cout << "x << 1        = " << x.operator <<(1)	<< std::endl;
	assert(x == 0x7);	// 0111

	y = for_bit_A;		// 1010
	assert((x & y) == 0x2);
}


/*
 *	Gate : Test basic Gate class.
 */
void testGate(void) {
	std::bitset<1>	*one_bit_pointer = new std::bitset<1>(0);
	std::bitset<2>	two_bits = 0,
					*two_bits_pointer = new std::bitset<2>(0);

	Gate<2>	g_with_null,
			g_with_1(&two_bits),
			*g_pointed = new Gate<2>(two_bits_pointer),
			g_ref(&two_bits);

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
	delete two_bits_pointer;
	assert(two_bits == 3);	// Source not deleted

	one_bit_pointer = g_ref.outRef();
	assert(g_ref.output() == *one_bit_pointer);
	assert(g_ref.outRef() == one_bit_pointer);
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
	ANDGate<4>	g4_0(&for_bit_0),
				g4_1(&for_bit_1),
				g4_8(&for_bit_8),
				g4_A(&for_bit_A),
				g4_F(&for_bit_F);

	// AND Gate with width 2 (2 inputs)
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

	// AND Gate with width 3 (3 inputs)
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

	// AND Gate with width 4 (4 inputs)
	assert(g4_0.output()			== 0);
	g4_0.tick();
	assert(g4_0.output()			== 0);

	assert(g4_1.output()			== 0);
	g4_1.tick();
	assert(g4_1.output()			== 0);

	assert(g4_8.output()			== 0);
	g4_8.tick();
	assert(g4_8.output()			== 0);

	assert(g4_A.output()			== 0);
	g4_A.tick();
	assert(g4_A.output()			== 0);

	assert(g4_F.output()			== 0);
	g4_F.tick();
	assert(g4_F.output()			== 1);	// Output changed to 1 after tick()
}

/*
 *	AND Gate : Test dynamic logic.
 */
void testLogic_AND_dynamic(void) {
	int i;
	std::bitset<1>	*one_bit_point2,
					*one_bit_point3;
	std::bitset<2>	two_bits = 0;
	std::bitset<3>	thr_bits = 0;

	ANDGate<2>	g2(&two_bits);
	ANDGate<3>	g3(&thr_bits);

	// Connect output to outside reference
	// assert (outside reference).value == gate.output.value
	one_bit_point2 = g2.outRef();
	one_bit_point3 = g3.outRef();

	g2.tick();
	assert(g2.output()		== 0);	// Output still 0
	assert(*one_bit_point2	== 0);
	two_bits = two_bit_3;
	assert(g2.output()		== 0);	// Output still 0
	assert(*one_bit_point2	== 0);
	g2.tick();
	assert(g2.output()		== 1);	// Output changed to 1 after tick()
	assert(*one_bit_point2	== 1);

	g3.tick();
	assert(g3.output()		== 0);	// Output still 0
	assert(*one_bit_point3	== 0);
	thr_bits = thr_bit_7;
	assert(g3.output()		== 0);	// Output still 0
	assert(*one_bit_point3	== 0);
	g3.tick();
	assert(g3.output()		== 1);	// Output changed to 1 after tick()
	assert(*one_bit_point3	== 1);

	for (two_bits = i = 0; i < 4; two_bits = ++i) {
		g2.tick();											// Update output
		assert(g2.output()		== (two_bits == two_bit_3));// Output change to 1 if two_bits == 11
		assert(*one_bit_point2	== (two_bits == two_bit_3));
	}

	for (thr_bits = i = 0; i < 8; thr_bits = ++i) {
		g3.tick();											// Update output
		assert(g3.output()		== (thr_bits == thr_bit_7));// Output change to 1 if thr_bits == 111
		assert(*one_bit_point3	== (thr_bits == thr_bit_7));
	}
}

/*
 *	NAND Gate : Test basic logic.
 */
void testLogic_NAND_const(void) {
	NANDGate<2>	g2_0(&two_bit_0),
				g2_1(&two_bit_1),
				g2_2(&two_bit_2),
				g2_3(&two_bit_3);
	NANDGate<3>	g3_0(&thr_bit_0),
				g3_1(&thr_bit_1),
				g3_2(&thr_bit_2),
				g3_3(&thr_bit_3),
				g3_4(&thr_bit_4),
				g3_5(&thr_bit_5),
				g3_6(&thr_bit_6),
				g3_7(&thr_bit_7);
	NANDGate<4>	g4_0(&for_bit_0),
				g4_1(&for_bit_1),
				g4_8(&for_bit_8),
				g4_A(&for_bit_A),
				g4_F(&for_bit_F);


	// NAND Gate with width 2 (2 inputs)
	assert(g2_0.output()			== 0);
	g2_0.tick();
	assert(g2_0.output()			== 1);

	assert(g2_1.output()			== 0);
	g2_1.tick();
	assert(g2_1.output()			== 1);

	assert(g2_2.output()			== 0);
	g2_2.tick();
	assert(g2_2.output()			== 1);

	assert(g2_3.output()			== 0);
	g2_3.tick();
	assert(g2_3.output()			== 0);	// Output stayed 0 after tick()

	// NAND Gate with width 3 (3 inputs)
	assert(g3_0.output()			== 0);
	g3_0.tick();
	assert(g3_0.output()			== 1);

	assert(g3_1.output()			== 0);
	g3_1.tick();
	assert(g3_1.output()			== 1);

	assert(g3_2.output()			== 0);
	g3_2.tick();
	assert(g3_2.output()			== 1);

	assert(g3_3.output()			== 0);
	g3_3.tick();
	assert(g3_3.output()			== 1);

	assert(g3_4.output()			== 0);
	g3_4.tick();
	assert(g3_4.output()			== 1);

	assert(g3_5.output()			== 0);
	g3_5.tick();
	assert(g3_5.output()			== 1);

	assert(g3_6.output()			== 0);
	g3_6.tick();
	assert(g3_6.output()			== 1);

	assert(g3_7.output()			== 0);
	g3_7.tick();
	assert(g3_7.output()			== 0);	// Output stayed 0 after tick()

	// NAND Gate with width 4 (4 inputs)
	assert(g4_0.output()			== 0);
	g4_0.tick();
	assert(g4_0.output()			== 1);

	assert(g4_1.output()			== 0);
	g4_1.tick();
	assert(g4_1.output()			== 1);

	assert(g4_8.output()			== 0);
	g4_8.tick();
	assert(g4_8.output()			== 1);

	assert(g4_A.output()			== 0);
	g4_A.tick();
	assert(g4_A.output()			== 1);

	assert(g4_F.output()			== 0);
	g4_F.tick();
	assert(g4_F.output()			== 0);	// Output stayed 0 after tick()
}

/*
 *	NAND Gate : Test dynamic logic.
 */
void testLogic_NAND_dynamic(void) {
	int i;
	std::bitset<1>	*one_bit_point2,
					*one_bit_point3;
	std::bitset<2>	two_bits = 0;
	std::bitset<3>	thr_bits = 0;

	NANDGate<2>	g2(&two_bits);
	NANDGate<3>	g3(&thr_bits);

	// Connect output to outside reference
	// assert (outside reference).value == gate.output.value
	one_bit_point2 = g2.outRef();
	one_bit_point3 = g3.outRef();

	g2.tick();
	assert(g2.output()		== 1);	// Output changed to 1 after tick()
	assert(*one_bit_point2	== 1);
	two_bits = two_bit_3;
	assert(g2.output()		== 1);	// Output still 1; input changed after tick()
	assert(*one_bit_point2	== 1);
	g2.tick();
	assert(g2.output()		== 0);	// Output changed to 0 after tick()
	assert(*one_bit_point2	== 0);

	g3.tick();
	assert(g3.output()		== 1);	// Output changed to 1 after tick()
	assert(*one_bit_point3	== 1);
	thr_bits = thr_bit_7;
	assert(g3.output()		== 1);	// Output still 1; input changed after tick()
	assert(*one_bit_point3	== 1);
	g3.tick();
	assert(g3.output()		== 0);	// Output changed to 0 after tick()
	assert(*one_bit_point3	== 0);

	for (two_bits = i = 0; i < 4; two_bits = ++i) {
		g2.tick();											// Update output
		assert(g2.output()		== (two_bits != two_bit_3));// Output change to 0 if two_bits == 11
		assert(*one_bit_point2	== (two_bits != two_bit_3));
	}

	for (thr_bits = i = 0; i < 8; thr_bits = ++i) {
		g3.tick();											// Update output
		assert(g3.output()		== (thr_bits != thr_bit_7));// Output change to 0 if thr_bits == 111
		assert(*one_bit_point3	== (thr_bits != thr_bit_7));
	}
}

/*
 *	OR Gate : Test basic logic.
 */
void testLogic_OR_const(void) {
	ORGate<2>	g2_0(&two_bit_0),
				g2_1(&two_bit_1),
				g2_2(&two_bit_2),
				g2_3(&two_bit_3);
	ORGate<3>	g3_0(&thr_bit_0),
				g3_1(&thr_bit_1),
				g3_2(&thr_bit_2),
				g3_3(&thr_bit_3),
				g3_4(&thr_bit_4),
				g3_5(&thr_bit_5),
				g3_6(&thr_bit_6),
				g3_7(&thr_bit_7);
	ORGate<4>	g4_0(&for_bit_0),
				g4_1(&for_bit_1),
				g4_8(&for_bit_8),
				g4_A(&for_bit_A),
				g4_F(&for_bit_F);

	// OR Gate with width 2 (2 inputs)
	assert(g2_0.output()			== 0);
	g2_0.tick();
	assert(g2_0.output()			== 0);

	assert(g2_1.output()			== 0);
	g2_1.tick();
	assert(g2_1.output()			== 1);

	assert(g2_2.output()			== 0);
	g2_2.tick();
	assert(g2_2.output()			== 1);

	assert(g2_3.output()			== 0);
	g2_3.tick();
	assert(g2_3.output()			== 1);	// Output changed to 1 after tick()

	// OR Gate with width 3 (3 inputs)
	assert(g3_0.output()			== 0);
	g3_0.tick();
	assert(g3_0.output()			== 0);

	assert(g3_1.output()			== 0);
	g3_1.tick();
	assert(g3_1.output()			== 1);

	assert(g3_2.output()			== 0);
	g3_2.tick();
	assert(g3_2.output()			== 1);

	assert(g3_3.output()			== 0);
	g3_3.tick();
	assert(g3_3.output()			== 1);

	assert(g3_4.output()			== 0);
	g3_4.tick();
	assert(g3_4.output()			== 1);

	assert(g3_5.output()			== 0);
	g3_5.tick();
	assert(g3_5.output()			== 1);

	assert(g3_6.output()			== 0);
	g3_6.tick();
	assert(g3_6.output()			== 1);

	assert(g3_7.output()			== 0);
	g3_7.tick();
	assert(g3_7.output()			== 1);	// Output changed to 1 after tick()

	// OR Gate with width 4 (4 inputs)
	assert(g4_0.output()			== 0);
	g4_0.tick();
	assert(g4_0.output()			== 0);

	assert(g4_1.output()			== 0);
	g4_1.tick();
	assert(g4_1.output()			== 1);

	assert(g4_8.output()			== 0);
	g4_8.tick();
	assert(g4_8.output()			== 1);

	assert(g4_A.output()			== 0);
	g4_A.tick();
	assert(g4_A.output()			== 1);

	assert(g4_F.output()			== 0);
	g4_F.tick();
	assert(g4_F.output()			== 1);	// Output changed to 1 after tick()
}

/*
 *	OR Gate : Test dynamic logic.
 */
void testLogic_OR_dynamic(void) {
	int i;
	std::bitset<1>	*one_bit_point2,
					*one_bit_point3;
	std::bitset<2>	two_bits = 0;
	std::bitset<3>	thr_bits = 0;

	ORGate<2>	g2(&two_bits);
	ORGate<3>	g3(&thr_bits);

	// Connect output to outside reference
	// assert (outside reference).value == gate.output.value
	one_bit_point2 = g2.outRef();
	one_bit_point3 = g3.outRef();

	g2.tick();
	assert(g2.output()		== 0);	// Output still 0
	assert(*one_bit_point2	== 0);
	two_bits = two_bit_3;
	assert(g2.output()		== 0);	// Output still 0
	assert(*one_bit_point2	== 0);
	g2.tick();
	assert(g2.output()		== 1);	// Output changed to 1 after tick()
	assert(*one_bit_point2	== 1);

	g3.tick();
	assert(g3.output()		== 0);	// Output still 0
	assert(*one_bit_point3	== 0);
	thr_bits = thr_bit_7;
	assert(g3.output()		== 0);	// Output still 0
	assert(*one_bit_point3	== 0);
	g3.tick();
	assert(g3.output()		== 1);	// Output changed to 1 after tick()
	assert(*one_bit_point3	== 1);

	for (two_bits = i = 0; i < 4; two_bits = ++i) {
		g2.tick();											// Update output
		assert(g2.output()		== (two_bits != two_bit_0));// Output change to 0 if two_bits == 00
		assert(*one_bit_point2	== (two_bits != two_bit_0));
	}

	for (thr_bits = i = 0; i < 8; thr_bits = ++i) {
		g3.tick();											// Update output
		assert(g3.output()		== (thr_bits != thr_bit_0));// Output change to 0 if thr_bits == 000
		assert(*one_bit_point3	== (thr_bits != thr_bit_0));
	}
}

/*
 *	NOR Gate : Test basic logic.
 */
void testLogic_NOR_const(void) {
	NORGate<2>	g2_0(&two_bit_0),
				g2_1(&two_bit_1),
				g2_2(&two_bit_2),
				g2_3(&two_bit_3);
	NORGate<3>	g3_0(&thr_bit_0),
				g3_1(&thr_bit_1),
				g3_2(&thr_bit_2),
				g3_3(&thr_bit_3),
				g3_4(&thr_bit_4),
				g3_5(&thr_bit_5),
				g3_6(&thr_bit_6),
				g3_7(&thr_bit_7);
	NORGate<4>	g4_0(&for_bit_0),
				g4_1(&for_bit_1),
				g4_8(&for_bit_8),
				g4_A(&for_bit_A),
				g4_F(&for_bit_F);

	// NOR Gate with width 2 (2 inputs)
	assert(g2_0.output()			== 0);
	g2_0.tick();
	assert(g2_0.output()			== 1);

	assert(g2_1.output()			== 0);
	g2_1.tick();
	assert(g2_1.output()			== 0);

	assert(g2_2.output()			== 0);
	g2_2.tick();
	assert(g2_2.output()			== 0);

	assert(g2_3.output()			== 0);
	g2_3.tick();
	assert(g2_3.output()			== 0);	// Output stayed 0 after tick()

	// NOR Gate with width 3 (3 inputs)
	assert(g3_0.output()			== 0);
	g3_0.tick();
	assert(g3_0.output()			== 1);

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
	assert(g3_7.output()			== 0);	// Output stayed 0 after tick()

	// NOR Gate with width 4 (4 inputs)
	assert(g4_0.output()			== 0);
	g4_0.tick();
	assert(g4_0.output()			== 1);

	assert(g4_1.output()			== 0);
	g4_1.tick();
	assert(g4_1.output()			== 0);

	assert(g4_8.output()			== 0);
	g4_8.tick();
	assert(g4_8.output()			== 0);

	assert(g4_A.output()			== 0);
	g4_A.tick();
	assert(g4_A.output()			== 0);

	assert(g4_F.output()			== 0);
	g4_F.tick();
	assert(g4_F.output()			== 0);	// Output stayed 1 after tick()
}

/*
 *	NOR Gate : Test dynamic logic.
 */
void testLogic_NOR_dynamic(void) {
	int i;
	std::bitset<1>	*one_bit_point2,
					*one_bit_point3;
	std::bitset<2>	two_bits = 0;
	std::bitset<3>	thr_bits = 0;

	NORGate<2>	g2(&two_bits);
	NORGate<3>	g3(&thr_bits);

	// Connect output to outside reference
	// assert (outside reference).value == gate.output.value
	one_bit_point2 = g2.outRef();
	one_bit_point3 = g3.outRef();

	g2.tick();
	assert(g2.output()		== 1);	// Output changed to 1 after tick()
	assert(*one_bit_point2	== 1);
	two_bits = two_bit_3;
	assert(g2.output()		== 1);	// Output still 1
	assert(*one_bit_point2	== 1);
	g2.tick();
	assert(g2.output()		== 0);	// Output changed to 0 after tick()
	assert(*one_bit_point2	== 0);

	g3.tick();
	assert(g3.output()		== 1);	// Output changed to 1 after tick()
	assert(*one_bit_point3	== 1);
	thr_bits = thr_bit_7;
	assert(g3.output()		== 1);	// Output still 1
	assert(*one_bit_point3	== 1);
	g3.tick();
	assert(g3.output()		== 0);	// Output changed to 0 after tick()
	assert(*one_bit_point3	== 0);

	for (two_bits = i = 0; i < 4; two_bits = ++i) {
		g2.tick();											// Update output
		assert(g2.output()		== (two_bits == two_bit_0));// Output change to 1 if two_bits == 00
		assert(*one_bit_point2	== (two_bits == two_bit_0));
	}

	for (thr_bits = i = 0; i < 8; thr_bits = ++i) {
		g3.tick();											// Update output
		assert(g3.output()		== (thr_bits == thr_bit_0));// Output change to 1 if thr_bits == 000
		assert(*one_bit_point3	== (thr_bits == thr_bit_0));
	}
}

/*
 *	XOR Gate : Test basic logic.
 */
void testLogic_XOR_const(void) {
	XORGate<2>	g2_0(&two_bit_0),
				g2_1(&two_bit_1),
				g2_2(&two_bit_2),
				g2_3(&two_bit_3);
	XORGate<3>	g3_0(&thr_bit_0),
				g3_1(&thr_bit_1),
				g3_2(&thr_bit_2),
				g3_3(&thr_bit_3),
				g3_4(&thr_bit_4),
				g3_5(&thr_bit_5),
				g3_6(&thr_bit_6),
				g3_7(&thr_bit_7);
	XORGate<4>	g4_0(&for_bit_0),
				g4_1(&for_bit_1),
				g4_8(&for_bit_8),
				g4_A(&for_bit_A),
				g4_F(&for_bit_F);

	// XOR Gate with width 2 (2 inputs)
	assert(g2_0.output()			== 0);
	g2_0.tick();
	assert(g2_0.output()			== 0);

	assert(g2_1.output()			== 0);
	g2_1.tick();
	assert(g2_1.output()			== 1);

	assert(g2_2.output()			== 0);
	g2_2.tick();
	assert(g2_2.output()			== 1);

	assert(g2_3.output()			== 0);
	g2_3.tick();
	assert(g2_3.output()			== 0);	// Output stayed 0 after tick()

	// XOR Gate with width 3 (3 inputs)
	assert(g3_0.output()			== 0);
	g3_0.tick();
	assert(g3_0.output()			== 0);

	assert(g3_1.output()			== 0);
	g3_1.tick();
	assert(g3_1.output()			== 1);

	assert(g3_2.output()			== 0);
	g3_2.tick();
	assert(g3_2.output()			== 1);

	assert(g3_3.output()			== 0);
	g3_3.tick();
	assert(g3_3.output()			== 0);

	assert(g3_4.output()			== 0);
	g3_4.tick();
	assert(g3_4.output()			== 1);

	assert(g3_5.output()			== 0);
	g3_5.tick();
	assert(g3_5.output()			== 0);

	assert(g3_6.output()			== 0);
	g3_6.tick();
	assert(g3_6.output()			== 0);

	assert(g3_7.output()			== 0);
	g3_7.tick();
	assert(g3_7.output()			== 1);	// Output changed to 1 after tick()

	// XOR Gate with width 4 (4 inputs)
	assert(g4_0.output()			== 0);
	g4_0.tick();
	assert(g4_0.output()			== 0);

	assert(g4_1.output()			== 0);
	g4_1.tick();
	assert(g4_1.output()			== 1);

	assert(g4_8.output()			== 0);
	g4_8.tick();
	assert(g4_8.output()			== 1);

	assert(g4_A.output()			== 0);
	g4_A.tick();
	assert(g4_A.output()			== 0);

	assert(g4_F.output()			== 0);
	g4_F.tick();
	assert(g4_F.output()			== 0);	// Output stayed 0 after tick()
}

/*
 *	XOR Gate : Test dynamic logic.
 */
void testLogic_XOR_dynamic(void) {
	int i;
	std::bitset<1>	*one_bit_point2,
					*one_bit_point3;
	std::bitset<2>	two_bits = 0;
	std::bitset<3>	thr_bits = 0;

	XORGate<2>	g2(&two_bits);
	XORGate<3>	g3(&thr_bits);

	// Connect output to outside reference
	// assert (outside reference).value == gate.output.value
	one_bit_point2 = g2.outRef();
	one_bit_point3 = g3.outRef();

	g2.tick();
	assert(g2.output()		== 0);	// Output still 0
	assert(*one_bit_point2	== 0);
	two_bits = two_bit_1;
	assert(g2.output()		== 0);	// Output still 0
	assert(*one_bit_point2	== 0);
	g2.tick();
	assert(g2.output()		== 1);	// Output changed to 1 after tick()
	assert(*one_bit_point2	== 1);

	g3.tick();
	assert(g3.output()		== 0);	// Output still 0
	assert(*one_bit_point3	== 0);
	thr_bits = thr_bit_7;
	assert(g3.output()		== 0);	// Output still 0
	assert(*one_bit_point3	== 0);
	g3.tick();
	assert(g3.output()		== 1);	// Output changed to 0 after tick()
	assert(*one_bit_point3	== 1);

	for (two_bits = i = 0; i < 4; two_bits = ++i) {
		g2.tick();												// Update output
		assert(g2.output()		== (two_bits.count() % 2 == 1));// Output change to 1 if two_bits has odd 1s
		assert(*one_bit_point2	== (two_bits.count() % 2 == 1));
	}

	for (thr_bits = i = 0; i < 8; thr_bits = ++i) {
		g3.tick();												// Update output
		assert(g3.output()		== (thr_bits.count() % 2 == 1));// Output change to 1 if thr_bits has odd 1s
		assert(*one_bit_point3	== (thr_bits.count() % 2 == 1));
	}
}

/*
 *	NOT Gate : Test basic logic.
 */
void testLogic_NOT_const(void) {
	NOTGate g1_0(&one_bit_0),
			g1_1(&one_bit_1);

	// NOT Gate with width 2 (2 inputs)
	assert(g1_0.output()			== 0);
	g1_0.tick();
	assert(g1_0.output()			== 1);

	assert(g1_1.output()			== 0);
	g1_1.tick();
	assert(g1_1.output()			== 0);	// Output stayed 0 after tick()
}

/*
 *	NOT Gate : Test dynamic logic.
 */
void testLogic_NOT_dynamic(void) {
	std::bitset<1>	one_bit = 0,
					*one_bit_point1;

	NOTGate g1(&one_bit);

	// Connect output to outside reference
	// assert (outside reference).value == gate.output.value
	one_bit_point1 = g1.outRef();

	g1.tick();
	assert(g1.output()		== 1);	// Output changed to 1 after tick()
	assert(*one_bit_point1	== 1);
	one_bit = one_bit_1;
	assert(g1.output()		== 1);	// Output still 1
	assert(*one_bit_point1	== 1);
	g1.tick();
	assert(g1.output()		== 0);	// Output changed to 0 after tick()
	assert(*one_bit_point1	== 0);
}

/*
 *	All gates : Test logic combinations.
 */
void testLogic_Combinations(void) {
	// TO-DO

	int i;
	std::bitset<2> two_bits = 0;

	// NOT(AND(x, y)) == NAND(x, y)
	ANDGate<2>	and_2(&two_bits);
	NOTGate		not_and_1(and_2.outRef());
	NANDGate<2>	nand_2(&two_bits);

	// NOT(OR(x, y)) == NOR(x, y)
	ORGate<2>	or_2(&two_bits);
	NOTGate		not_or_1(or_2.outRef());
	NORGate<2>	nor_2(&two_bits);

	for (two_bits = i = 0; i < 4; two_bits = ++i) {
		// NOT(AND(x, y)) == NAND(x, y)
		and_2.tick();									// Update output
		not_and_1.tick();
		nand_2.tick();
		assert(not_and_1.output() == nand_2.output());	// Output Not(And) must equal Nand

		// NOT(OR(x, y)) == NOR(x, y)
		or_2.tick();									// Update output
		not_or_1.tick();
		nor_2.tick();
		assert(not_or_1.output() == nor_2.output());	// Output Not(Or) must equal Nor
	}
}


void testMultiGate(void) {
	int i;
	std::bitset<1>	one_bit1 = 0, one_bit2 = 1, one_bit3 = 0, one_bit4 = 0,
					*bitarr[4] = {&one_bit1, &one_bit2, &one_bit3, &one_bit4},
					*one_bit_pointer = new std::bitset<1>(0);
	std::bitset<2>	two_bits = 0,
					*two_bits_pointer = new std::bitset<2>(0);

	MultiGate<1, 1> g1_1_with_arr(bitarr);
	MultiGate<1, 2>	g1_2_with_1(&two_bits),
					*g1_2_pointed = new MultiGate<1, 2>(two_bits_pointer),
					g1_2_ref(&two_bits);
	MultiGate<4, 1> g4_1(bitarr),
					g4_1_with_null;

	assert(g1_1_with_arr.allInputsSet()			== true);
	assert(g1_1_with_arr.getInputAmount()		== 1);
	assert(g1_1_with_arr.getInput()				== bitarr[0]);
	assert(*g1_1_with_arr.getInput()			== one_bit1);
	one_bit1 = 1;
	assert(*g1_1_with_arr.getInput()			== one_bit_1);
	assert(g1_1_with_arr.getOutput().size()		== 1);
	assert(g1_1_with_arr.getOutput()			== 0);
	assert(g1_1_with_arr.getMultiOutput().size()== 1);
	assert(g1_1_with_arr.getMultiOutput()		== 0);

	assert(g1_2_with_1.allInputsSet()			== true);
	assert(g1_2_with_1.getInputAmount()			== 1);
	assert(g1_2_with_1.getInput()				== &two_bits);
	two_bits = 2;
	assert(*g1_2_with_1.getInput()				== two_bit_2);
	assert(g1_2_with_1.getOutput().size()		== 1);
	assert(g1_2_with_1.getOutput()				== 0);
	assert(g1_2_with_1.getMultiOutput().size()	== 2);
	assert(g1_2_with_1.getMultiOutput()			== 0);

	assert(g1_2_pointed != nullptr);
	assert(g1_2_with_1.allInputsSet()			== true);
	assert(g1_2_pointed->getInputAmount()		== 1);
	assert(g1_2_pointed->getInput()				== two_bits_pointer);
	*two_bits_pointer = 2;
	assert(*g1_2_pointed->getInput()			== two_bit_2);
	assert(g1_2_pointed->getOutput().size()		== 1);
	assert(g1_2_pointed->getOutput()			== 0);
	assert(g1_2_pointed->getMultiOutput().size()== 2);
	assert(g1_2_pointed->getMultiOutput()		== 0);

	one_bit_pointer = g1_2_ref.getOutRef();
	assert(g1_2_ref.getOutput()				== *one_bit_pointer);
	assert(g1_2_ref.getOutRef()				== one_bit_pointer);
	two_bits_pointer = g1_2_ref.getMultiOutRef();
	assert(g1_2_ref.getMultiOutput()		== *two_bits_pointer);
	assert(g1_2_ref.getMultiOutRef()		== two_bits_pointer);

	assert(g4_1.allInputsSet()				== true);
	assert(g4_1.getInputAmount()			== 4);
	for (i = 0; i < 4; i++) {
		assert(g4_1.getInput(i)				== bitarr[i]);
		assert(*g4_1.getInput(i)			== *bitarr[i]);
	}
	one_bit3 = one_bit4 = 1;
	for (i = 0; i < 4; i++) {
		assert(*g4_1.getInput(i)			== one_bit_1);
	}

	assert(g4_1_with_null.allInputsSet()	== false);
	assert(g4_1_with_null.getInputAmount()	== 4);
	g4_1_with_null.addInput(&one_bit1);
	g4_1_with_null.addInput(&one_bit1);
	g4_1_with_null.addInput(&one_bit1);
	g4_1_with_null.addInput(&one_bit2);
	for (i = 0; i < 4; i++) {
		assert(g4_1_with_null.getInput(i)	== (i == 3 ? &one_bit2 : &one_bit1));
		assert(*g4_1_with_null.getInput(i)	== one_bit_1);
	}
	one_bit1 = 0;
	for (i = 0; i < 4; i++) {
		assert(*g4_1_with_null.getInput(i)	== (i == 3 ? one_bit_1 : one_bit_0));
	}

	delete one_bit_pointer;
	delete g1_2_pointed;
}

/*
 *	AND Gate : Test basic logic.
 */
void testLogic_MultiAND_const(void) {
	MultiANDGate<1, 2>	g1_2_0(&two_bit_0),
						g1_2_1(&two_bit_1),
						g1_2_2(&two_bit_2),
						g1_2_3(&two_bit_3);
	MultiANDGate<1, 3>	g1_3_0(&thr_bit_0),
						g1_3_1(&thr_bit_1),
						g1_3_2(&thr_bit_2),
						g1_3_3(&thr_bit_3),
						g1_3_4(&thr_bit_4),
						g1_3_5(&thr_bit_5),
						g1_3_6(&thr_bit_6),
						g1_3_7(&thr_bit_7);
	MultiANDGate<1, 4>	g1_4_0(&for_bit_0),
						g1_4_1(&for_bit_1),
						g1_4_8(&for_bit_8),
						g1_4_A(&for_bit_A),
						g1_4_F(&for_bit_F);

	// AND Gate with width 2 (1 set of 2 inputs)
	assert(g1_2_0.getOutput()		== 0);
	g1_2_0.tick();
	assert(g1_2_0.getOutput()		== 0);

	assert(g1_2_1.getOutput()		== 0);
	g1_2_1.tick();
	assert(g1_2_1.getOutput()		== 0);

	assert(g1_2_2.getOutput()		== 0);
	g1_2_2.tick();
	assert(g1_2_2.getOutput()		== 0);

	assert(g1_2_3.getOutput()		== 0);
	g1_2_3.tick();
	assert(g1_2_3.getOutput()		== 1);	// Output changed to 1 after tick()

	// AND Gate with width 3 (1 set of 3 inputs)
	assert(g1_3_0.getOutput()		== 0);
	g1_3_0.tick();
	assert(g1_3_0.getOutput()		== 0);

	assert(g1_3_1.getOutput()		== 0);
	g1_3_1.tick();
	assert(g1_3_1.getOutput()		== 0);

	assert(g1_3_2.getOutput()		== 0);
	g1_3_2.tick();
	assert(g1_3_2.getOutput()		== 0);

	assert(g1_3_3.getOutput()		== 0);
	g1_3_3.tick();
	assert(g1_3_3.getOutput()		== 0);

	assert(g1_3_4.getOutput()		== 0);
	g1_3_4.tick();
	assert(g1_3_4.getOutput()		== 0);

	assert(g1_3_5.getOutput()		== 0);
	g1_3_5.tick();
	assert(g1_3_5.getOutput()		== 0);

	assert(g1_3_6.getOutput()		== 0);
	g1_3_6.tick();
	assert(g1_3_6.getOutput()		== 0);

	assert(g1_3_7.getOutput()		== 0);
	g1_3_7.tick();
	assert(g1_3_7.getOutput()		== 1);	// Output changed to 1 after tick()

	// AND Gate with width 4 (1 set of 4 inputs)
	assert(g1_4_0.getOutput()		== 0);
	g1_4_0.tick();
	assert(g1_4_0.getOutput()		== 0);

	assert(g1_4_1.getOutput()		== 0);
	g1_4_1.tick();
	assert(g1_4_1.getOutput()		== 0);

	assert(g1_4_8.getOutput()		== 0);
	g1_4_8.tick();
	assert(g1_4_8.getOutput()		== 0);

	assert(g1_4_A.getOutput()		== 0);
	g1_4_A.tick();
	assert(g1_4_A.getOutput()		== 0);

	assert(g1_4_F.getOutput()		== 0);
	g1_4_F.tick();
	assert(g1_4_F.getOutput()		== 1);	// Output changed to 1 after tick()
}

/*
 *	AND Gate : Test bus logic.
 */
void testLogic_MultiAND_bus(void) {
	bitset<1>	bit1_0 = 0, bit2_0 = 0,
				bit1_1 = 1, bit2_1 = 1,
				*bit2_1_0_arr[2]	= { &bit1_0, &bit2_0},
				*bit2_1_1_arr[2]	= { &bit1_1, &bit2_1};
	bitset<3>	bit3_3 = 3,
				*bit3_3_arr[3]		= { &bit3_3, &thr_bit_2, &thr_bit_7};

	MultiANDGate<2, 1>	g2_1_0(bit2_1_0_arr),
						g2_1_1(bit2_1_1_arr);
	MultiANDGate<3, 3>	g3_3(bit3_3_arr);

	// AND Gate with width 1 (2 sets of 1 input)
	assert(g2_1_0.getMultiOutput()	== 0);
	g2_1_0.tick();
	assert(g2_1_0.getMultiOutput()	== 0);

	assert(g2_1_1.getMultiOutput()	== 0);
	g2_1_1.tick();
	assert(g2_1_1.getOutput()		== 1);
	assert(g2_1_1.getMultiOutput()	== 1);	// Output changed to 1 after tick()

	assert(g3_3.getMultiOutput()	== thr_bit_0);
	g3_3.tick();
	assert(g3_3.getMultiOutput()	== thr_bit_2);
	bit3_3 = 0;
	assert(g3_3.getMultiOutput()	== thr_bit_2);
	g3_3.tick();
	assert(g3_3.getMultiOutput()	== thr_bit_0);


	bitset<8>	b8_2 = 2,
				*b20_8_arr[20] = { &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2,
								   &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2};

	MultiANDGate<20, 8> g20_8(b20_8_arr);

	assert(g20_8.getMultiOutput()	== 0);
	g20_8.tick();
	assert(g20_8.getMultiOutput()	== 2);
	b8_2 = 0xF0;
	assert(g20_8.getMultiOutput()	== 2);
	g20_8.tick();
	assert(g20_8.getMultiOutput()	== 0xF0);
}

/*
 *	Run all tests.
 */
void runTests(void) {
	int errorlevel = 0;

	try {
		testBitset();
		testGate();
		testLogic_AND_const();
		testLogic_AND_dynamic();
		testLogic_NAND_const();
		testLogic_NAND_dynamic();
		testLogic_OR_const();
		testLogic_OR_dynamic();
		testLogic_NOR_const();
		testLogic_NOR_dynamic();
		testLogic_XOR_const();
		testLogic_XOR_dynamic();
		testLogic_NOT_const();
		testLogic_NOT_dynamic();
		testLogic_Combinations();

		testMultiGate();
		testLogic_MultiAND_const();
		testLogic_MultiAND_bus();

	} catch (Exceptions::Exception const& e) {
		std::cerr << e.getMessage() << std::endl;
		errorlevel++;
	} catch(...) {
		std::cerr << "Unknown exception." << std::endl;
		errorlevel++;
	}


	std::cout << (errorlevel ? "There were errors running tests. See message(s) above." :
							  "All tests passed.") << std::endl;
}


#endif // ifndef NDEBUG

#endif // UNITTEST_H
