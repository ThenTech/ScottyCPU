#ifndef UNITTEST_HPP
#define UNITTEST_HPP

#ifndef NDEBUG		// ifndef NDEBUG

#include <iostream>
#include <cassert>	// Debug assertion
#include <bitset>
#include "Exceptions.hpp"

#include "SynchrotronComponent.hpp"

#include "ANDGate.hpp"
//#include "NANDGate.hpp"
//#include "ORGate.hpp"
//#include "NORGate.hpp"
//#include "XORGate.hpp"
//#include "NOTGate.hpp"

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
						for_bit_2 = 2,	// 0010
						for_bit_3 = 3,	// 0011
						for_bit_4 = 4,	// 0100
						for_bit_8 = 8,	// 1000
						for_bit_A = 10,	// 1010
						for_bit_B = 11,	// 1011
						for_bit_F = 15;	// 1111

/*
 *	Constant signals and signal providers.
 */
static SynchrotronComponent<1>	signal1_0(one_bit_0.to_ulong()),					// 0
								signal1_0_(one_bit_0.to_ulong()),					// 0 other unique
								signal1_1(one_bit_1.to_ulong()),						// 1
								signal1_1_(one_bit_1.to_ulong());					// 1 other unique
static SynchrotronComponent<2>	signal2_0(two_bit_0.to_ulong()),					// 00
								signal2_0_(two_bit_0.to_ulong()),					// 00 other unique
								signal2_1(two_bit_1.to_ulong()),					// 01
								signal2_1_(two_bit_1.to_ulong()),					// 01 other unique
								signal2_2(two_bit_2.to_ulong()),					// 10
								signal2_3(two_bit_3.to_ulong());					// 11
static SynchrotronComponent<1>	signalProvider1_0_0( {&signal1_0, &signal1_0_} ),	// 0 0
								signalProvider1_0_1( {&signal1_0, &signal1_1} ),	// 0 1
								signalProvider1_1_0( {&signal1_1, &signal1_0} ),	// 1 0
								signalProvider1_1_1( {&signal1_1, &signal1_1_} );	// 1 1
static SynchrotronComponent<2>	signalProvider2_0_0( {&signal2_0, &signal2_0_} ),	// 00 00
								signalProvider2_0_1( {&signal2_0, &signal2_1} ),	// 00 01
								signalProvider2_1_0( {&signal2_1, &signal2_0} ),	// 01 00
								signalProvider2_1_1( {&signal2_1, &signal2_1_} ),	// 01 01
								signalProvider2_0_2( {&signal2_0, &signal2_2} ),	// 00 10
								signalProvider2_1_2( {&signal2_1, &signal2_2} ),	// 01 10
								signalProvider2_0_3( {&signal2_0, &signal2_3} ),	// 00 11
								signalProvider2_1_3( {&signal2_1, &signal2_3} );	// 01 11

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
 *	SynchrotronComponent : Test SynchrotronComponent class.
 */
void testSynchrotronComponent(void) {
	SynchrotronComponent<4>	s_with_null,
							s_with_1(for_bit_1.to_ulong()),
							s_with_2(for_bit_2.to_ulong()),
							s_sign_1(for_bit_1.to_ulong()),
							*s_pointed = new SynchrotronComponent<4>(for_bit_2.to_ulong());

	assert(s_pointed != nullptr);

	assert(s_with_null.getState()				== for_bit_0);
	assert(s_with_null.getBitWidth()			== 4);
	assert(s_with_null.getIputs().size()		== 0);
	assert(s_with_null.getOutputs().size()		== 0);
	assert(s_with_1.getState()					== for_bit_1);
	assert(s_with_1.getBitWidth()				== 4);
	assert(s_with_1.getIputs().size()			== 0);
	assert(s_with_1.getOutputs().size()			== 0);
	assert(s_pointed->getState()				== for_bit_2);
	assert(s_pointed->getBitWidth()				== 4);
	assert(s_pointed->getIputs().size()			== 0);
	assert(s_pointed->getOutputs().size()		== 0);

	s_with_null.addOutput( {&s_with_1, &s_with_2} );
	assert(s_with_null.getOutputs().size()		== 2);
	s_with_null.addOutput(s_with_1); // Add again to see if size() increases, it should not.
	assert(s_with_null.getOutputs().size()		== 2);
	assert(s_with_1.getIputs().size()			== 1);
	s_with_1.addInput(s_with_null); // Add again to see if size() increases, it should not.
	assert(s_with_1.getIputs().size()			== 1);
	assert(s_with_2.getIputs().size()			== 1);

	s_with_null.emit();	// Internal State is 0x0, gets ORed to subscribers
	assert(s_with_1.getState()					== for_bit_1);

	s_with_null.addInput(*s_pointed);
	assert(s_with_null.getIputs().size()		== 1);
	assert(s_pointed->getOutputs().size()		== 1);
	s_pointed->emit(); // Internal State is 0x2, gets ORed to subscribers
	assert(s_with_null.getState()				== for_bit_2); // tick()ed by s_pointed
	assert(s_with_1.getState()					== for_bit_3); // tick()ed by s_with_null = 0x2 | 0x1
	assert(s_with_2.getState()					== for_bit_2);

	s_with_1.removeInput(s_with_null);
	assert(s_with_1.getIputs().size()			== 0);
	assert(s_with_null.getOutputs().size()		== 1);
	s_with_null.removeOutput(s_with_1); // Remove again to see if size() decreases, it should not.
	assert(s_with_null.getOutputs().size()		== 1);

	s_sign_1.addOutput(s_with_null);
	assert(s_with_null.getIputs().size()		== 2);
	assert(s_sign_1.getOutputs().size()			== 1);
	s_with_null.tick();	// Should update from 0x2 to 0x3 (==> s_sign_1(1) | s_pointed(2) == 3)
	assert(s_with_null.getState()				== for_bit_3);
	assert(s_with_1.getState()					== for_bit_3); // unchanged
	assert(s_with_2.getState()					== for_bit_3); // tick()ed by s_with_null = 0x3 | 0x2

	// flow: {s_pointed, s_sign_1} -> s_with_null -> s_with_2
	SynchrotronComponent<4>	s_copy_sign(s_with_2);
	// flow: {s_pointed, s_sign_1} -> s_with_null -> {s_with_2, s_copy_sign}

	assert(s_copy_sign.getIputs().size()		== 1);
	assert(s_copy_sign.getOutputs().size()		== 0);
	assert(s_with_null.getOutputs().size()		== 2);
	assert(s_copy_sign.getState()				== for_bit_0);
	s_with_null.emit();
	assert(s_copy_sign.getState()				== for_bit_3);

	{	// Create components in block to check dtors after block
		SynchrotronComponent<4>	s_copy_both(s_with_null, true),
								s_with_8(for_bit_8.to_ulong());
		// flow: {s_pointed, s_sign_1} -> {s_with_null, s_copy_both} -> {s_with_2, s_copy_sign}
		s_copy_both.addInput(s_with_8);
		// flow:              s_with_8 -> s_copy_both

		assert(s_with_8.getIputs().size()		== 0);
		assert(s_with_8.getOutputs().size()		== 1);
		assert(s_copy_both.getIputs().size()	== 3);
		assert(s_copy_both.getOutputs().size()	== 2);
		assert(s_copy_sign.getIputs().size()	== 2);
		assert(s_with_2.getIputs().size()		== 2);
		assert(s_pointed->getOutputs().size()	== 2);
		assert(s_sign_1.getOutputs().size()		== 2);
		s_with_8.emit();
		assert(s_copy_both.getState()			== for_bit_B); // tick()ed by s_with_8    = 0x0 | 0x8 | 0x3
		assert(s_with_2.getState()				== for_bit_B); // tick()ed by s_copy_both = 0x3 | 0x8
		assert(s_copy_sign.getState()			== for_bit_B); // tick()ed by s_copy_both = 0x3 | 0x8
	}	// s_copy_both and s_with_8 destructed => connections should be gone

	// flow: {s_pointed, s_sign_1} -> s_with_null -> {s_with_2, s_copy_sign}
	assert(s_pointed->getOutputs().size()		== 1);
	assert(s_sign_1.getOutputs().size()			== 1);
	assert(s_copy_sign.getIputs().size()		== 1);
	assert(s_with_2.getIputs().size()			== 1);

	delete s_pointed;
}

/*
 *	AND Gate : Test basic logic.
 */
void testLogic_AND_const(void) {
	ANDGate<1>	g1_0_0(signalProvider1_0_0),
				g1_0_1(signalProvider1_0_1),
				g1_1_0(signalProvider1_1_0),
				g1_1_1(signalProvider1_1_1);
	ANDGate<2>	g2_0_0(signalProvider2_0_0),
				g2_0_1(signalProvider2_0_1),
				g2_1_0(signalProvider2_1_0),
				g2_1_1(signalProvider2_1_1),
				g2_0_2(signalProvider2_0_2),
				g2_1_2(signalProvider2_1_2),
				g2_0_3(signalProvider2_0_3),
				g2_1_3(signalProvider2_1_3);

	// AND Gate with width 1 (2 inputs)
	assert(g1_0_0.getState()			== one_bit_0);
	g1_0_0.tick();
	assert(g1_0_0.getState()			== one_bit_0);

	assert(g1_0_1.getState()			== one_bit_0);
	g1_0_1.tick();
	assert(g1_0_1.getState()			== one_bit_0);

	assert(g1_1_0.getState()			== one_bit_0);
	g1_1_0.tick();
	assert(g1_1_0.getState()			== one_bit_0);

	assert(g1_1_1.getState()			== one_bit_0);
	g1_1_1.tick();
	assert(g1_1_1.getState()			== one_bit_1);	// Output changed to 1 after tick()

	// AND Gate with width 2 (2 inputs)
	assert(g2_0_0.getState()			== two_bit_0);
	g2_0_0.tick();
	assert(g2_0_0.getState()			== two_bit_0);

	assert(g2_0_1.getState()			== two_bit_0);
	g2_0_1.tick();
	assert(g2_0_1.getState()			== two_bit_0);

	assert(g2_1_0.getState()			== two_bit_0);
	g2_1_0.tick();
	assert(g2_1_0.getState()			== two_bit_0);

	assert(g2_1_1.getState()			== two_bit_0);
	g2_1_1.tick();
	assert(g2_1_1.getState()			== two_bit_1);	// Output changed to 1 after tick()

	assert(g2_0_2.getState()			== two_bit_0);
	g2_0_2.tick();
	assert(g2_0_2.getState()			== two_bit_0);

	assert(g2_1_2.getState()			== two_bit_0);
	g2_1_2.tick();
	assert(g2_1_2.getState()			== two_bit_0);

	assert(g2_0_3.getState()			== two_bit_0);
	g2_0_3.tick();
	assert(g2_0_3.getState()			== two_bit_0);

	assert(g2_1_3.getState()			== two_bit_0);
	g2_1_3.tick();
	assert(g2_1_3.getState()			== two_bit_1);	// Output changed to 1 after tick()
}

/*
 *	AND Gate : Test dynamic logic.
 */
void testLogic_AND_dynamic(void) {
	ANDGate<2> g2;

	g2.addInput( {&signal2_0, &signal2_0_} );
	assert(g2.getIputs().size()			== 2);
	g2.tick();
	assert(g2.getState()				== two_bit_0);
	g2.removeInput(signal2_0);
	g2.removeInput(signal2_0_);
	assert(g2.getIputs().size()			== 0);

	g2.addInput( {&signal2_1, &signal2_2} );
	assert(g2.getIputs().size()			== 2);
	g2.tick();
	assert(g2.getState()				== two_bit_0);
	g2.removeInput(signal2_2);
	assert(g2.getIputs().size()			== 1);

	g2.addInput(signal2_3);
	assert(g2.getIputs().size()			== 2);
	g2.tick();
	assert(g2.getState()				== two_bit_1);
	g2.removeInput(signal2_1);
	g2.removeInput(signal2_3);
	assert(g2.getIputs().size()			== 0);
}

///*
// *	NAND Gate : Test basic logic.
// */
//void testLogic_NAND_const(void) {
//	NANDGate<2>	g2_0(&two_bit_0),
//				g2_1(&two_bit_1),
//				g2_2(&two_bit_2),
//				g2_3(&two_bit_3);
//	NANDGate<3>	g3_0(&thr_bit_0),
//				g3_1(&thr_bit_1),
//				g3_2(&thr_bit_2),
//				g3_3(&thr_bit_3),
//				g3_4(&thr_bit_4),
//				g3_5(&thr_bit_5),
//				g3_6(&thr_bit_6),
//				g3_7(&thr_bit_7);
//	NANDGate<4>	g4_0(&for_bit_0),
//				g4_1(&for_bit_1),
//				g4_8(&for_bit_8),
//				g4_A(&for_bit_A),
//				g4_F(&for_bit_F);
//
//
//	// NAND Gate with width 2 (2 inputs)
//	assert(g2_0.output()			== 0);
//	g2_0.tick();
//	assert(g2_0.output()			== 1);
//
//	assert(g2_1.output()			== 0);
//	g2_1.tick();
//	assert(g2_1.output()			== 1);
//
//	assert(g2_2.output()			== 0);
//	g2_2.tick();
//	assert(g2_2.output()			== 1);
//
//	assert(g2_3.output()			== 0);
//	g2_3.tick();
//	assert(g2_3.output()			== 0);	// Output stayed 0 after tick()
//
//	// NAND Gate with width 3 (3 inputs)
//	assert(g3_0.output()			== 0);
//	g3_0.tick();
//	assert(g3_0.output()			== 1);
//
//	assert(g3_1.output()			== 0);
//	g3_1.tick();
//	assert(g3_1.output()			== 1);
//
//	assert(g3_2.output()			== 0);
//	g3_2.tick();
//	assert(g3_2.output()			== 1);
//
//	assert(g3_3.output()			== 0);
//	g3_3.tick();
//	assert(g3_3.output()			== 1);
//
//	assert(g3_4.output()			== 0);
//	g3_4.tick();
//	assert(g3_4.output()			== 1);
//
//	assert(g3_5.output()			== 0);
//	g3_5.tick();
//	assert(g3_5.output()			== 1);
//
//	assert(g3_6.output()			== 0);
//	g3_6.tick();
//	assert(g3_6.output()			== 1);
//
//	assert(g3_7.output()			== 0);
//	g3_7.tick();
//	assert(g3_7.output()			== 0);	// Output stayed 0 after tick()
//
//	// NAND Gate with width 4 (4 inputs)
//	assert(g4_0.output()			== 0);
//	g4_0.tick();
//	assert(g4_0.output()			== 1);
//
//	assert(g4_1.output()			== 0);
//	g4_1.tick();
//	assert(g4_1.output()			== 1);
//
//	assert(g4_8.output()			== 0);
//	g4_8.tick();
//	assert(g4_8.output()			== 1);
//
//	assert(g4_A.output()			== 0);
//	g4_A.tick();
//	assert(g4_A.output()			== 1);
//
//	assert(g4_F.output()			== 0);
//	g4_F.tick();
//	assert(g4_F.output()			== 0);	// Output stayed 0 after tick()
//}
//
///*
// *	NAND Gate : Test dynamic logic.
// */
//void testLogic_NAND_dynamic(void) {
//	int i;
//	std::bitset<1>	*one_bit_point2,
//					*one_bit_point3;
//	std::bitset<2>	two_bits = 0;
//	std::bitset<3>	thr_bits = 0;
//
//	NANDGate<2>	g2(&two_bits);
//	NANDGate<3>	g3(&thr_bits);
//
//	// Connect output to outside reference
//	// assert (outside reference).value == gate.output.value
//	one_bit_point2 = g2.outRef();
//	one_bit_point3 = g3.outRef();
//
//	g2.tick();
//	assert(g2.output()		== 1);	// Output changed to 1 after tick()
//	assert(*one_bit_point2	== 1);
//	two_bits = two_bit_3;
//	assert(g2.output()		== 1);	// Output still 1; input changed after tick()
//	assert(*one_bit_point2	== 1);
//	g2.tick();
//	assert(g2.output()		== 0);	// Output changed to 0 after tick()
//	assert(*one_bit_point2	== 0);
//
//	g3.tick();
//	assert(g3.output()		== 1);	// Output changed to 1 after tick()
//	assert(*one_bit_point3	== 1);
//	thr_bits = thr_bit_7;
//	assert(g3.output()		== 1);	// Output still 1; input changed after tick()
//	assert(*one_bit_point3	== 1);
//	g3.tick();
//	assert(g3.output()		== 0);	// Output changed to 0 after tick()
//	assert(*one_bit_point3	== 0);
//
//	for (two_bits = i = 0; i < 4; two_bits = ++i) {
//		g2.tick();											// Update output
//		assert(g2.output()		== (two_bits != two_bit_3));// Output change to 0 if two_bits == 11
//		assert(*one_bit_point2	== (two_bits != two_bit_3));
//	}
//
//	for (thr_bits = i = 0; i < 8; thr_bits = ++i) {
//		g3.tick();											// Update output
//		assert(g3.output()		== (thr_bits != thr_bit_7));// Output change to 0 if thr_bits == 111
//		assert(*one_bit_point3	== (thr_bits != thr_bit_7));
//	}
//}
//
///*
// *	OR Gate : Test basic logic.
// */
//void testLogic_OR_const(void) {
//	ORGate<2>	g2_0(&two_bit_0),
//				g2_1(&two_bit_1),
//				g2_2(&two_bit_2),
//				g2_3(&two_bit_3);
//	ORGate<3>	g3_0(&thr_bit_0),
//				g3_1(&thr_bit_1),
//				g3_2(&thr_bit_2),
//				g3_3(&thr_bit_3),
//				g3_4(&thr_bit_4),
//				g3_5(&thr_bit_5),
//				g3_6(&thr_bit_6),
//				g3_7(&thr_bit_7);
//	ORGate<4>	g4_0(&for_bit_0),
//				g4_1(&for_bit_1),
//				g4_8(&for_bit_8),
//				g4_A(&for_bit_A),
//				g4_F(&for_bit_F);
//
//	// OR Gate with width 2 (2 inputs)
//	assert(g2_0.output()			== 0);
//	g2_0.tick();
//	assert(g2_0.output()			== 0);
//
//	assert(g2_1.output()			== 0);
//	g2_1.tick();
//	assert(g2_1.output()			== 1);
//
//	assert(g2_2.output()			== 0);
//	g2_2.tick();
//	assert(g2_2.output()			== 1);
//
//	assert(g2_3.output()			== 0);
//	g2_3.tick();
//	assert(g2_3.output()			== 1);	// Output changed to 1 after tick()
//
//	// OR Gate with width 3 (3 inputs)
//	assert(g3_0.output()			== 0);
//	g3_0.tick();
//	assert(g3_0.output()			== 0);
//
//	assert(g3_1.output()			== 0);
//	g3_1.tick();
//	assert(g3_1.output()			== 1);
//
//	assert(g3_2.output()			== 0);
//	g3_2.tick();
//	assert(g3_2.output()			== 1);
//
//	assert(g3_3.output()			== 0);
//	g3_3.tick();
//	assert(g3_3.output()			== 1);
//
//	assert(g3_4.output()			== 0);
//	g3_4.tick();
//	assert(g3_4.output()			== 1);
//
//	assert(g3_5.output()			== 0);
//	g3_5.tick();
//	assert(g3_5.output()			== 1);
//
//	assert(g3_6.output()			== 0);
//	g3_6.tick();
//	assert(g3_6.output()			== 1);
//
//	assert(g3_7.output()			== 0);
//	g3_7.tick();
//	assert(g3_7.output()			== 1);	// Output changed to 1 after tick()
//
//	// OR Gate with width 4 (4 inputs)
//	assert(g4_0.output()			== 0);
//	g4_0.tick();
//	assert(g4_0.output()			== 0);
//
//	assert(g4_1.output()			== 0);
//	g4_1.tick();
//	assert(g4_1.output()			== 1);
//
//	assert(g4_8.output()			== 0);
//	g4_8.tick();
//	assert(g4_8.output()			== 1);
//
//	assert(g4_A.output()			== 0);
//	g4_A.tick();
//	assert(g4_A.output()			== 1);
//
//	assert(g4_F.output()			== 0);
//	g4_F.tick();
//	assert(g4_F.output()			== 1);	// Output changed to 1 after tick()
//}
//
///*
// *	OR Gate : Test dynamic logic.
// */
//void testLogic_OR_dynamic(void) {
//	int i;
//	std::bitset<1>	*one_bit_point2,
//					*one_bit_point3;
//	std::bitset<2>	two_bits = 0;
//	std::bitset<3>	thr_bits = 0;
//
//	ORGate<2>	g2(&two_bits);
//	ORGate<3>	g3(&thr_bits);
//
//	// Connect output to outside reference
//	// assert (outside reference).value == gate.output.value
//	one_bit_point2 = g2.outRef();
//	one_bit_point3 = g3.outRef();
//
//	g2.tick();
//	assert(g2.output()		== 0);	// Output still 0
//	assert(*one_bit_point2	== 0);
//	two_bits = two_bit_3;
//	assert(g2.output()		== 0);	// Output still 0
//	assert(*one_bit_point2	== 0);
//	g2.tick();
//	assert(g2.output()		== 1);	// Output changed to 1 after tick()
//	assert(*one_bit_point2	== 1);
//
//	g3.tick();
//	assert(g3.output()		== 0);	// Output still 0
//	assert(*one_bit_point3	== 0);
//	thr_bits = thr_bit_7;
//	assert(g3.output()		== 0);	// Output still 0
//	assert(*one_bit_point3	== 0);
//	g3.tick();
//	assert(g3.output()		== 1);	// Output changed to 1 after tick()
//	assert(*one_bit_point3	== 1);
//
//	for (two_bits = i = 0; i < 4; two_bits = ++i) {
//		g2.tick();											// Update output
//		assert(g2.output()		== (two_bits != two_bit_0));// Output change to 0 if two_bits == 00
//		assert(*one_bit_point2	== (two_bits != two_bit_0));
//	}
//
//	for (thr_bits = i = 0; i < 8; thr_bits = ++i) {
//		g3.tick();											// Update output
//		assert(g3.output()		== (thr_bits != thr_bit_0));// Output change to 0 if thr_bits == 000
//		assert(*one_bit_point3	== (thr_bits != thr_bit_0));
//	}
//}
//
///*
// *	NOR Gate : Test basic logic.
// */
//void testLogic_NOR_const(void) {
//	NORGate<2>	g2_0(&two_bit_0),
//				g2_1(&two_bit_1),
//				g2_2(&two_bit_2),
//				g2_3(&two_bit_3);
//	NORGate<3>	g3_0(&thr_bit_0),
//				g3_1(&thr_bit_1),
//				g3_2(&thr_bit_2),
//				g3_3(&thr_bit_3),
//				g3_4(&thr_bit_4),
//				g3_5(&thr_bit_5),
//				g3_6(&thr_bit_6),
//				g3_7(&thr_bit_7);
//	NORGate<4>	g4_0(&for_bit_0),
//				g4_1(&for_bit_1),
//				g4_8(&for_bit_8),
//				g4_A(&for_bit_A),
//				g4_F(&for_bit_F);
//
//	// NOR Gate with width 2 (2 inputs)
//	assert(g2_0.output()			== 0);
//	g2_0.tick();
//	assert(g2_0.output()			== 1);
//
//	assert(g2_1.output()			== 0);
//	g2_1.tick();
//	assert(g2_1.output()			== 0);
//
//	assert(g2_2.output()			== 0);
//	g2_2.tick();
//	assert(g2_2.output()			== 0);
//
//	assert(g2_3.output()			== 0);
//	g2_3.tick();
//	assert(g2_3.output()			== 0);	// Output stayed 0 after tick()
//
//	// NOR Gate with width 3 (3 inputs)
//	assert(g3_0.output()			== 0);
//	g3_0.tick();
//	assert(g3_0.output()			== 1);
//
//	assert(g3_1.output()			== 0);
//	g3_1.tick();
//	assert(g3_1.output()			== 0);
//
//	assert(g3_2.output()			== 0);
//	g3_2.tick();
//	assert(g3_2.output()			== 0);
//
//	assert(g3_3.output()			== 0);
//	g3_3.tick();
//	assert(g3_3.output()			== 0);
//
//	assert(g3_4.output()			== 0);
//	g3_4.tick();
//	assert(g3_4.output()			== 0);
//
//	assert(g3_5.output()			== 0);
//	g3_5.tick();
//	assert(g3_5.output()			== 0);
//
//	assert(g3_6.output()			== 0);
//	g3_6.tick();
//	assert(g3_6.output()			== 0);
//
//	assert(g3_7.output()			== 0);
//	g3_7.tick();
//	assert(g3_7.output()			== 0);	// Output stayed 0 after tick()
//
//	// NOR Gate with width 4 (4 inputs)
//	assert(g4_0.output()			== 0);
//	g4_0.tick();
//	assert(g4_0.output()			== 1);
//
//	assert(g4_1.output()			== 0);
//	g4_1.tick();
//	assert(g4_1.output()			== 0);
//
//	assert(g4_8.output()			== 0);
//	g4_8.tick();
//	assert(g4_8.output()			== 0);
//
//	assert(g4_A.output()			== 0);
//	g4_A.tick();
//	assert(g4_A.output()			== 0);
//
//	assert(g4_F.output()			== 0);
//	g4_F.tick();
//	assert(g4_F.output()			== 0);	// Output stayed 1 after tick()
//}
//
///*
// *	NOR Gate : Test dynamic logic.
// */
//void testLogic_NOR_dynamic(void) {
//	int i;
//	std::bitset<1>	*one_bit_point2,
//					*one_bit_point3;
//	std::bitset<2>	two_bits = 0;
//	std::bitset<3>	thr_bits = 0;
//
//	NORGate<2>	g2(&two_bits);
//	NORGate<3>	g3(&thr_bits);
//
//	// Connect output to outside reference
//	// assert (outside reference).value == gate.output.value
//	one_bit_point2 = g2.outRef();
//	one_bit_point3 = g3.outRef();
//
//	g2.tick();
//	assert(g2.output()		== 1);	// Output changed to 1 after tick()
//	assert(*one_bit_point2	== 1);
//	two_bits = two_bit_3;
//	assert(g2.output()		== 1);	// Output still 1
//	assert(*one_bit_point2	== 1);
//	g2.tick();
//	assert(g2.output()		== 0);	// Output changed to 0 after tick()
//	assert(*one_bit_point2	== 0);
//
//	g3.tick();
//	assert(g3.output()		== 1);	// Output changed to 1 after tick()
//	assert(*one_bit_point3	== 1);
//	thr_bits = thr_bit_7;
//	assert(g3.output()		== 1);	// Output still 1
//	assert(*one_bit_point3	== 1);
//	g3.tick();
//	assert(g3.output()		== 0);	// Output changed to 0 after tick()
//	assert(*one_bit_point3	== 0);
//
//	for (two_bits = i = 0; i < 4; two_bits = ++i) {
//		g2.tick();											// Update output
//		assert(g2.output()		== (two_bits == two_bit_0));// Output change to 1 if two_bits == 00
//		assert(*one_bit_point2	== (two_bits == two_bit_0));
//	}
//
//	for (thr_bits = i = 0; i < 8; thr_bits = ++i) {
//		g3.tick();											// Update output
//		assert(g3.output()		== (thr_bits == thr_bit_0));// Output change to 1 if thr_bits == 000
//		assert(*one_bit_point3	== (thr_bits == thr_bit_0));
//	}
//}
//
///*
// *	XOR Gate : Test basic logic.
// */
//void testLogic_XOR_const(void) {
//	XORGate<2>	g2_0(&two_bit_0),
//				g2_1(&two_bit_1),
//				g2_2(&two_bit_2),
//				g2_3(&two_bit_3);
//	XORGate<3>	g3_0(&thr_bit_0),
//				g3_1(&thr_bit_1),
//				g3_2(&thr_bit_2),
//				g3_3(&thr_bit_3),
//				g3_4(&thr_bit_4),
//				g3_5(&thr_bit_5),
//				g3_6(&thr_bit_6),
//				g3_7(&thr_bit_7);
//	XORGate<4>	g4_0(&for_bit_0),
//				g4_1(&for_bit_1),
//				g4_8(&for_bit_8),
//				g4_A(&for_bit_A),
//				g4_F(&for_bit_F);
//
//	// XOR Gate with width 2 (2 inputs)
//	assert(g2_0.output()			== 0);
//	g2_0.tick();
//	assert(g2_0.output()			== 0);
//
//	assert(g2_1.output()			== 0);
//	g2_1.tick();
//	assert(g2_1.output()			== 1);
//
//	assert(g2_2.output()			== 0);
//	g2_2.tick();
//	assert(g2_2.output()			== 1);
//
//	assert(g2_3.output()			== 0);
//	g2_3.tick();
//	assert(g2_3.output()			== 0);	// Output stayed 0 after tick()
//
//	// XOR Gate with width 3 (3 inputs)
//	assert(g3_0.output()			== 0);
//	g3_0.tick();
//	assert(g3_0.output()			== 0);
//
//	assert(g3_1.output()			== 0);
//	g3_1.tick();
//	assert(g3_1.output()			== 1);
//
//	assert(g3_2.output()			== 0);
//	g3_2.tick();
//	assert(g3_2.output()			== 1);
//
//	assert(g3_3.output()			== 0);
//	g3_3.tick();
//	assert(g3_3.output()			== 0);
//
//	assert(g3_4.output()			== 0);
//	g3_4.tick();
//	assert(g3_4.output()			== 1);
//
//	assert(g3_5.output()			== 0);
//	g3_5.tick();
//	assert(g3_5.output()			== 0);
//
//	assert(g3_6.output()			== 0);
//	g3_6.tick();
//	assert(g3_6.output()			== 0);
//
//	assert(g3_7.output()			== 0);
//	g3_7.tick();
//	assert(g3_7.output()			== 1);	// Output changed to 1 after tick()
//
//	// XOR Gate with width 4 (4 inputs)
//	assert(g4_0.output()			== 0);
//	g4_0.tick();
//	assert(g4_0.output()			== 0);
//
//	assert(g4_1.output()			== 0);
//	g4_1.tick();
//	assert(g4_1.output()			== 1);
//
//	assert(g4_8.output()			== 0);
//	g4_8.tick();
//	assert(g4_8.output()			== 1);
//
//	assert(g4_A.output()			== 0);
//	g4_A.tick();
//	assert(g4_A.output()			== 0);
//
//	assert(g4_F.output()			== 0);
//	g4_F.tick();
//	assert(g4_F.output()			== 0);	// Output stayed 0 after tick()
//}
//
///*
// *	XOR Gate : Test dynamic logic.
// */
//void testLogic_XOR_dynamic(void) {
//	int i;
//	std::bitset<1>	*one_bit_point2,
//					*one_bit_point3;
//	std::bitset<2>	two_bits = 0;
//	std::bitset<3>	thr_bits = 0;
//
//	XORGate<2>	g2(&two_bits);
//	XORGate<3>	g3(&thr_bits);
//
//	// Connect output to outside reference
//	// assert (outside reference).value == gate.output.value
//	one_bit_point2 = g2.outRef();
//	one_bit_point3 = g3.outRef();
//
//	g2.tick();
//	assert(g2.output()		== 0);	// Output still 0
//	assert(*one_bit_point2	== 0);
//	two_bits = two_bit_1;
//	assert(g2.output()		== 0);	// Output still 0
//	assert(*one_bit_point2	== 0);
//	g2.tick();
//	assert(g2.output()		== 1);	// Output changed to 1 after tick()
//	assert(*one_bit_point2	== 1);
//
//	g3.tick();
//	assert(g3.output()		== 0);	// Output still 0
//	assert(*one_bit_point3	== 0);
//	thr_bits = thr_bit_7;
//	assert(g3.output()		== 0);	// Output still 0
//	assert(*one_bit_point3	== 0);
//	g3.tick();
//	assert(g3.output()		== 1);	// Output changed to 0 after tick()
//	assert(*one_bit_point3	== 1);
//
//	for (two_bits = i = 0; i < 4; two_bits = ++i) {
//		g2.tick();												// Update output
//		assert(g2.output()		== (two_bits.count() % 2 == 1));// Output change to 1 if two_bits has odd 1s
//		assert(*one_bit_point2	== (two_bits.count() % 2 == 1));
//	}
//
//	for (thr_bits = i = 0; i < 8; thr_bits = ++i) {
//		g3.tick();												// Update output
//		assert(g3.output()		== (thr_bits.count() % 2 == 1));// Output change to 1 if thr_bits has odd 1s
//		assert(*one_bit_point3	== (thr_bits.count() % 2 == 1));
//	}
//}
//
///*
// *	NOT Gate : Test basic logic.
// */
//void testLogic_NOT_const(void) {
//	NOTGate g1_0(&one_bit_0),
//			g1_1(&one_bit_1);
//
//	// NOT Gate with width 2 (2 inputs)
//	assert(g1_0.output()			== 0);
//	g1_0.tick();
//	assert(g1_0.output()			== 1);
//
//	assert(g1_1.output()			== 0);
//	g1_1.tick();
//	assert(g1_1.output()			== 0);	// Output stayed 0 after tick()
//}
//
///*
// *	NOT Gate : Test dynamic logic.
// */
//void testLogic_NOT_dynamic(void) {
//	std::bitset<1>	one_bit = 0,
//					*one_bit_point1;
//
//	NOTGate g1(&one_bit);
//
//	// Connect output to outside reference
//	// assert (outside reference).value == gate.output.value
//	one_bit_point1 = g1.outRef();
//
//	g1.tick();
//	assert(g1.output()		== 1);	// Output changed to 1 after tick()
//	assert(*one_bit_point1	== 1);
//	one_bit = one_bit_1;
//	assert(g1.output()		== 1);	// Output still 1
//	assert(*one_bit_point1	== 1);
//	g1.tick();
//	assert(g1.output()		== 0);	// Output changed to 0 after tick()
//	assert(*one_bit_point1	== 0);
//}
//
///*
// *	All gates : Test logic combinations.
// */
//void testLogic_Combinations(void) {
//	// TO-DO
//
//	int i;
//	std::bitset<2> two_bits = 0;
//
//	// NOT(AND(x, y)) == NAND(x, y)
//	ANDGate<2>	and_2(&two_bits);
//	NOTGate		not_and_1(and_2.outRef());
//	NANDGate<2>	nand_2(&two_bits);
//
//	// NOT(OR(x, y)) == NOR(x, y)
//	ORGate<2>	or_2(&two_bits);
//	NOTGate		not_or_1(or_2.outRef());
//	NORGate<2>	nor_2(&two_bits);
//
//	for (two_bits = i = 0; i < 4; two_bits = ++i) {
//		// NOT(AND(x, y)) == NAND(x, y)
//		and_2.tick();									// Update output
//		not_and_1.tick();
//		nand_2.tick();
//		assert(not_and_1.output() == nand_2.output());	// Output Not(And) must equal Nand
//
//		// NOT(OR(x, y)) == NOR(x, y)
//		or_2.tick();									// Update output
//		not_or_1.tick();
//		nor_2.tick();
//		assert(not_or_1.output() == nor_2.output());	// Output Not(Or) must equal Nor
//	}
//}
//
//
//void testMultiGate(void) {
//	int i;
//	std::bitset<1>	one_bit1 = 0, one_bit2 = 1, one_bit3 = 0, one_bit4 = 0,
//					*bitarr[4] = {&one_bit1, &one_bit2, &one_bit3, &one_bit4},
//					*one_bit_pointer = new std::bitset<1>(0);
//	std::bitset<2>	two_bits = 0,
//					*two_bits_pointer = new std::bitset<2>(0);
//
//	MultiGate<1, 1> g1_1_with_arr(bitarr);
//	MultiGate<1, 2>	g1_2_with_1(&two_bits),
//					*g1_2_pointed = new MultiGate<1, 2>(two_bits_pointer),
//					g1_2_ref(&two_bits);
//	MultiGate<4, 1> g4_1(bitarr),
//					g4_1_with_null;
//
//	assert(g1_1_with_arr.allInputsSet()			== true);
//	assert(g1_1_with_arr.getInputAmount()		== 1);
//	assert(g1_1_with_arr.getInput()				== bitarr[0]);
//	assert(*g1_1_with_arr.getInput()			== one_bit1);
//	one_bit1 = 1;
//	assert(*g1_1_with_arr.getInput()			== one_bit_1);
//	assert(g1_1_with_arr.getOutput().size()		== 1);
//	assert(g1_1_with_arr.getOutput()			== 0);
//	assert(g1_1_with_arr.getMultiOutput().size()== 1);
//	assert(g1_1_with_arr.getMultiOutput()		== 0);
//
//	assert(g1_2_with_1.allInputsSet()			== true);
//	assert(g1_2_with_1.getInputAmount()			== 1);
//	assert(g1_2_with_1.getInput()				== &two_bits);
//	two_bits = 2;
//	assert(*g1_2_with_1.getInput()				== two_bit_2);
//	assert(g1_2_with_1.getOutput().size()		== 1);
//	assert(g1_2_with_1.getOutput()				== 0);
//	assert(g1_2_with_1.getMultiOutput().size()	== 2);
//	assert(g1_2_with_1.getMultiOutput()			== 0);
//
//	assert(g1_2_pointed != nullptr);
//	assert(g1_2_with_1.allInputsSet()			== true);
//	assert(g1_2_pointed->getInputAmount()		== 1);
//	assert(g1_2_pointed->getInput()				== two_bits_pointer);
//	*two_bits_pointer = 2;
//	assert(*g1_2_pointed->getInput()			== two_bit_2);
//	assert(g1_2_pointed->getOutput().size()		== 1);
//	assert(g1_2_pointed->getOutput()			== 0);
//	assert(g1_2_pointed->getMultiOutput().size()== 2);
//	assert(g1_2_pointed->getMultiOutput()		== 0);
//
//	one_bit_pointer = g1_2_ref.getOutRef();
//	assert(g1_2_ref.getOutput()				== *one_bit_pointer);
//	assert(g1_2_ref.getOutRef()				== one_bit_pointer);
//	two_bits_pointer = g1_2_ref.getMultiOutRef();
//	assert(g1_2_ref.getMultiOutput()		== *two_bits_pointer);
//	assert(g1_2_ref.getMultiOutRef()		== two_bits_pointer);
//
//	assert(g4_1.allInputsSet()				== true);
//	assert(g4_1.getInputAmount()			== 4);
//	for (i = 0; i < 4; i++) {
//		assert(g4_1.getInput(i)				== bitarr[i]);
//		assert(*g4_1.getInput(i)			== *bitarr[i]);
//	}
//	one_bit3 = one_bit4 = 1;
//	for (i = 0; i < 4; i++) {
//		assert(*g4_1.getInput(i)			== one_bit_1);
//	}
//
//	assert(g4_1_with_null.allInputsSet()	== false);
//	assert(g4_1_with_null.getInputAmount()	== 4);
//	g4_1_with_null.addInput(&one_bit1);
//	g4_1_with_null.addInput(&one_bit1);
//	g4_1_with_null.addInput(&one_bit1);
//	g4_1_with_null.addInput(&one_bit2);
//	for (i = 0; i < 4; i++) {
//		assert(g4_1_with_null.getInput(i)	== (i == 3 ? &one_bit2 : &one_bit1));
//		assert(*g4_1_with_null.getInput(i)	== one_bit_1);
//	}
//	one_bit1 = 0;
//	for (i = 0; i < 4; i++) {
//		assert(*g4_1_with_null.getInput(i)	== (i == 3 ? one_bit_1 : one_bit_0));
//	}
//
//	delete one_bit_pointer;
//	delete g1_2_pointed;
//}
//
///*
// *	AND Gate : Test basic logic.
// */
//void testLogic_MultiAND_const(void) {
//	MultiANDGate<1, 2>	g1_2_0(&two_bit_0),
//						g1_2_1(&two_bit_1),
//						g1_2_2(&two_bit_2),
//						g1_2_3(&two_bit_3);
//	MultiANDGate<1, 3>	g1_3_0(&thr_bit_0),
//						g1_3_1(&thr_bit_1),
//						g1_3_2(&thr_bit_2),
//						g1_3_3(&thr_bit_3),
//						g1_3_4(&thr_bit_4),
//						g1_3_5(&thr_bit_5),
//						g1_3_6(&thr_bit_6),
//						g1_3_7(&thr_bit_7);
//	MultiANDGate<1, 4>	g1_4_0(&for_bit_0),
//						g1_4_1(&for_bit_1),
//						g1_4_8(&for_bit_8),
//						g1_4_A(&for_bit_A),
//						g1_4_F(&for_bit_F);
//
//	// AND Gate with width 2 (1 set of 2 inputs)
//	assert(g1_2_0.getOutput()		== 0);
//	g1_2_0.tick();
//	assert(g1_2_0.getOutput()		== 0);
//
//	assert(g1_2_1.getOutput()		== 0);
//	g1_2_1.tick();
//	assert(g1_2_1.getOutput()		== 0);
//
//	assert(g1_2_2.getOutput()		== 0);
//	g1_2_2.tick();
//	assert(g1_2_2.getOutput()		== 0);
//
//	assert(g1_2_3.getOutput()		== 0);
//	g1_2_3.tick();
//	assert(g1_2_3.getOutput()		== 1);	// Output changed to 1 after tick()
//
//	// AND Gate with width 3 (1 set of 3 inputs)
//	assert(g1_3_0.getOutput()		== 0);
//	g1_3_0.tick();
//	assert(g1_3_0.getOutput()		== 0);
//
//	assert(g1_3_1.getOutput()		== 0);
//	g1_3_1.tick();
//	assert(g1_3_1.getOutput()		== 0);
//
//	assert(g1_3_2.getOutput()		== 0);
//	g1_3_2.tick();
//	assert(g1_3_2.getOutput()		== 0);
//
//	assert(g1_3_3.getOutput()		== 0);
//	g1_3_3.tick();
//	assert(g1_3_3.getOutput()		== 0);
//
//	assert(g1_3_4.getOutput()		== 0);
//	g1_3_4.tick();
//	assert(g1_3_4.getOutput()		== 0);
//
//	assert(g1_3_5.getOutput()		== 0);
//	g1_3_5.tick();
//	assert(g1_3_5.getOutput()		== 0);
//
//	assert(g1_3_6.getOutput()		== 0);
//	g1_3_6.tick();
//	assert(g1_3_6.getOutput()		== 0);
//
//	assert(g1_3_7.getOutput()		== 0);
//	g1_3_7.tick();
//	assert(g1_3_7.getOutput()		== 1);	// Output changed to 1 after tick()
//
//	// AND Gate with width 4 (1 set of 4 inputs)
//	assert(g1_4_0.getOutput()		== 0);
//	g1_4_0.tick();
//	assert(g1_4_0.getOutput()		== 0);
//
//	assert(g1_4_1.getOutput()		== 0);
//	g1_4_1.tick();
//	assert(g1_4_1.getOutput()		== 0);
//
//	assert(g1_4_8.getOutput()		== 0);
//	g1_4_8.tick();
//	assert(g1_4_8.getOutput()		== 0);
//
//	assert(g1_4_A.getOutput()		== 0);
//	g1_4_A.tick();
//	assert(g1_4_A.getOutput()		== 0);
//
//	assert(g1_4_F.getOutput()		== 0);
//	g1_4_F.tick();
//	assert(g1_4_F.getOutput()		== 1);	// Output changed to 1 after tick()
//}
//
///*
// *	AND Gate : Test bus logic.
// */
//void testLogic_MultiAND_bus(void) {
//	bitset<1>	bit1_0 = 0, bit2_0 = 0,
//				bit1_1 = 1, bit2_1 = 1,
//				*bit2_1_0_arr[2]	= { &bit1_0, &bit2_0},
//				*bit2_1_1_arr[2]	= { &bit1_1, &bit2_1};
//	bitset<3>	bit3_3 = 3,
//				*bit3_3_arr[3]		= { &bit3_3, &thr_bit_2, &thr_bit_7};
//
//	MultiANDGate<2, 1>	g2_1_0(bit2_1_0_arr),
//						g2_1_1(bit2_1_1_arr);
//	MultiANDGate<3, 3>	g3_3(bit3_3_arr);
//
//	// AND Gate with width 1 (2 sets of 1 input)
//	assert(g2_1_0.getMultiOutput()	== 0);
//	g2_1_0.tick();
//	assert(g2_1_0.getMultiOutput()	== 0);
//
//	assert(g2_1_1.getMultiOutput()	== 0);
//	g2_1_1.tick();
//	assert(g2_1_1.getOutput()		== 1);
//	assert(g2_1_1.getMultiOutput()	== 1);	// Output changed to 1 after tick()
//
//	assert(g3_3.getMultiOutput()	== thr_bit_0);
//	g3_3.tick();
//	assert(g3_3.getMultiOutput()	== thr_bit_2);
//	bit3_3 = 0;
//	assert(g3_3.getMultiOutput()	== thr_bit_2);
//	g3_3.tick();
//	assert(g3_3.getMultiOutput()	== thr_bit_0);
//
//
//	bitset<8>	b8_2 = 2,
//				*b20_8_arr[20] = { &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2,
//								   &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2, &b8_2};
//
//	MultiANDGate<20, 8> g20_8(b20_8_arr);
//
//	assert(g20_8.getMultiOutput()	== 0);
//	g20_8.tick();
//	assert(g20_8.getMultiOutput()	== 2);
//	b8_2 = 0xF0;
//	assert(g20_8.getMultiOutput()	== 2);
//	g20_8.tick();
//	assert(g20_8.getMultiOutput()	== 0xF0);
//}

/*
 *	Run all tests.
 */
void runTests(void) {
	int errorlevel = 0;

	try {
		testBitset();
		testSynchrotronComponent();
		testLogic_AND_const();
		testLogic_AND_dynamic();
//		testLogic_NAND_const();
//		testLogic_NAND_dynamic();
//		testLogic_OR_const();
//		testLogic_OR_dynamic();
//		testLogic_NOR_const();
//		testLogic_NOR_dynamic();
//		testLogic_XOR_const();
//		testLogic_XOR_dynamic();
//		testLogic_NOT_const();
//		testLogic_NOT_dynamic();
//		testLogic_Combinations();
//
//		testMultiGate();
//		testLogic_MultiAND_const();
//		testLogic_MultiAND_bus();

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

#endif // UNITTEST_HPP
