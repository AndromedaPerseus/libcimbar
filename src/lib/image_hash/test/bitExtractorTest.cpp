/* This code is subject to the terms of the Mozilla Public License, v.2.0. http://mozilla.org/MPL/2.0/. */
#include "unittest.h"

#include "bit_extractor.h"
#include "intx/intx.hpp"

#include <bitset>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;

TEST_CASE( "bitExtractorTest/testDefault.4", "[unit]" )
{
	uint64_t bits = 0x11d07e1f;
	bit_extractor<uint64_t, 32, 4> be(bits);

	// we're just reading 4 bits at a time.
	assertEquals( 0x1d, be.extract(0, 8) );    // 0001 1101
	assertEquals( 0x71, be.extract(16, 24) );  // 0x7 0x1
	assertEquals( 0x1e, be.extract(0, 20) );
	assertEquals( 0x0f, be.extract(12, 28) );
	assertEquals( 0x1d, be.extract(24, 8) );
}


TEST_CASE( "bitExtractorTest/testDefault.5", "[unit]" )
{
	uint64_t bits = 0x11d07e1f;
	bit_extractor<uint64_t, 32, 5> be(bits);

	assertEquals(  0x48, be.extract(0, 10) );   // 00010 01000
	assertEquals(  0xfc, be.extract(15, 20) );  // 00111 11100
	assertEquals(  0x9c, be.extract(1, 20) );   // 00100 11100
	assertEquals( 0x1e7, be.extract(16, 25) );  // 01111 00111
}

TEST_CASE( "bitExtractorTest/testDefault.8", "[unit]" )
{
	uint64_t bits = 0x11d07e1f;
	bit_extractor<uint64_t, 32, 8> be(bits);

	assertEquals( 0x11d0, be.extract(0, 8) );
	assertEquals( 0x7e1f, be.extract(16, 24) );
	assertEquals( 0x111f, be.extract(0, 24) );
	assertEquals( 0x1fd0, be.extract(24, 8) );
}


TEST_CASE( "bitExtractorTest/testLargerValue.1", "[unit]" )
{
	intx::uint128 bits(0);
	for (int i = 0; i < 81; ++i)
		bits = (bits << 1) | (i%3 == 0);

	bit_extractor<intx::uint128, 81, 8> be(bits);
	uint64_t res = be.extract(0, 9, 18, 27, 36, 45, 54, 63);
	assertEquals( 0x9292929292929292, res );
}

TEST_CASE( "bitExtractorTest/testLargerValue.2", "[unit]" )
{
	intx::uint128 bits{0xF83C0E030080000ULL, 0xFFBFCFE3FULL};
	assertEquals( "ffbfcfe3f0f83c0e030080000", intx::hex(bits) );  // sanity check

	bit_extractor<intx::uint128, 100, 8> be(bits);
	uint64_t res = be.extract(1, 11, 21, 31);
	assertEquals( 0xfffefcf8, res );

	res = be.extract(41, 51, 61, 71);
	assertEquals( 0xf0e0c080, res );

	res = be.extract(1, 11, 21, 31, 41, 51, 61, 71);
	assertEquals( 0xfffefcf8f0e0c080ULL, res );

	res = be.extract(22, 32, 42, 52);
	assertEquals( 0xf8f0e0c0, res );

	res = be.extract(22, 32, 42, 52, 62, 72, 82, 92);
	assertEquals( 0xf8f0e0c080000000ULL, res );
}

