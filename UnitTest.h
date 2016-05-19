#ifndef UNITTEST_H
#define UNITTEST_H

#ifndef NDEBUG		// ifndef NDEBUG

#include <iostream>
#include <cassert>	// Debug assertion
#include "Gate.h"
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

/*
 *	Run all tests.
 */
void runTests(void) {
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
}


#endif // ifndef NDEBUG

#endif // UNITTEST_H
