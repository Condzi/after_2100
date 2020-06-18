#include <catch/catch.hpp>
#include <engine/formatting.hpp>

#include <sstream>
#include <string>

// @Robustness: We're leaking memory here. Not much, but still.
TEST_CASE( "sprint", "engine" )
{
	using namespace con;

	SECTION( "f32" )
	{
		Temporary_Allocator ta;
		Default_Allocator da;
		Context.default_allocator = &da;
		Context.temporary_allocator = &ta;
		da.initialize();
		ta.initialize();

		f32 const value_0     = 0.0f;
		f32 const value_plus  = 123.321f;
		f32 const value_minus = -567.765f;

		CString const value_0_str     = f32_to_cstring( value_0 );
		CString const value_plus_str  = f32_to_cstring( value_plus );
		CString const value_minus_str = f32_to_cstring( value_minus );

		REQUIRE( value_0_str.size == 1 );
		REQUIRE( value_0_str.data[0] == '0' );

		REQUIRE( value_plus_str.size == 7 );
		REQUIRE( value_plus_str.data[0] == '1' );
		REQUIRE( value_plus_str.data[1] == '2' );
		REQUIRE( value_plus_str.data[2] == '3' );
		REQUIRE( value_plus_str.data[3] == '.' );
		REQUIRE( value_plus_str.data[4] == '3' );
		REQUIRE( value_plus_str.data[5] == '2' );
		REQUIRE( value_plus_str.data[6] == '1' );

		REQUIRE( value_minus_str.size == 8 );
		REQUIRE( value_minus_str.data[0] == '-' );
		REQUIRE( value_minus_str.data[1] == '5' );
		REQUIRE( value_minus_str.data[2] == '6' );
		REQUIRE( value_minus_str.data[3] == '7' );
		REQUIRE( value_minus_str.data[4] == '.' );
		REQUIRE( value_minus_str.data[5] == '7' );
		REQUIRE( value_minus_str.data[6] == '6' );
		REQUIRE( value_minus_str.data[7] == '5' );


		Context.default_allocator = nullptr;
		Context.temporary_allocator = nullptr;
		da.shutdown();
	}

	SECTION( "s32" )
	{
		Temporary_Allocator ta;
		Default_Allocator da;
		Context.default_allocator = &da;
		Context.temporary_allocator = &ta;
		da.initialize();
		ta.initialize();


		s32 const value_0     = 0;
		s32 const value_plus  = 123456789;
		s32 const value_minus = -987654321;

		CString const value_0_str     = s32_to_cstring( value_0 );
		CString const value_plus_str  = s32_to_cstring( value_plus );
		CString const value_minus_str = s32_to_cstring( value_minus );

		REQUIRE( value_0_str.size == 1 );
		REQUIRE( value_0_str.data[0] == '0' );

		REQUIRE( value_plus_str.size == 9 );
		REQUIRE( value_plus_str.data[0] == '1' );
		REQUIRE( value_plus_str.data[1] == '2' );
		REQUIRE( value_plus_str.data[2] == '3' );
		REQUIRE( value_plus_str.data[3] == '4' );
		REQUIRE( value_plus_str.data[4] == '5' );
		REQUIRE( value_plus_str.data[5] == '6' );
		REQUIRE( value_plus_str.data[6] == '7' );
		REQUIRE( value_plus_str.data[7] == '8' );
		REQUIRE( value_plus_str.data[8] == '9' );


		REQUIRE( value_minus_str.size == 10 );
		REQUIRE( value_minus_str.data[0] == '-' );
		REQUIRE( value_minus_str.data[1] == '9' );
		REQUIRE( value_minus_str.data[2] == '8' );
		REQUIRE( value_minus_str.data[3] == '7' );
		REQUIRE( value_minus_str.data[4] == '6' );
		REQUIRE( value_minus_str.data[5] == '5' );
		REQUIRE( value_minus_str.data[6] == '4' );
		REQUIRE( value_minus_str.data[7] == '3' );
		REQUIRE( value_minus_str.data[8] == '2' );
		REQUIRE( value_minus_str.data[9] == '1' );


		Context.default_allocator = nullptr;
		Context.temporary_allocator = nullptr;
		da.shutdown();
	}

	SECTION( "sprint" )
	{
		Temporary_Allocator ta;
		Default_Allocator da;
		Context.default_allocator = &da;
		Context.temporary_allocator = &ta;
		da.initialize();
		ta.initialize();

		constant str = sprint( "[START OF SPRINT TEST]\n\tvalue_s32 = %\n\tvalue_f32 = %\n\tvalue_str = %.\n"_cs, 1337, 21.37, "Oh, hi Mark!"_cs );

		for ( s32 i = 0; i < str.size; ++i ) {
			putchar( str.data[i] );
		}

		printf( "\tMemory used: %i / %i bytes.\n[END OF SPRINT TEST]\n\n", ta.get_mark(), CON_TEMPORARY_STORAGE_RESERVED_MEMORY );

		Context.default_allocator = nullptr;
		Context.temporary_allocator = nullptr;
		da.shutdown();
	}
}

TEST_CASE( "CString to T", "engine" )
{
	using namespace con;

	s32 const s32_value = 42069420;
	f32 const f32_value = 6969.69420f;
	CString const s32_str = "42069420";
	CString const f32_str = "6969.69420";

	s32 const s32_converted = cstring_to_s32( s32_str );
	f32 const f32_converted = cstring_to_f32( f32_str );

	REQUIRE( s32_converted == s32_value );
	REQUIRE( f32_converted == f32_value );
}

TEST_CASE( "sscan", "engine" )
{
	using namespace con;

	Temporary_Allocator ta;
	Default_Allocator da;
	Context.default_allocator = &da;
	Context.temporary_allocator = &ta;
	da.initialize();
	ta.initialize();

	{
		s32 const s32_value = 4202;
		f32 const f32_value = 69.09f;

		CString const str_1    = "4202 69.09";
		CString const format_1 = "% %";

		CString const str_2    = "blah 69.09! 4202! bleh";
		CString const format_2 = "blah %! %! bleh";

		s32 s32_1 = 0;
		s32 s32_2 = 0;
		f32 f32_1 = 0;
		f32 f32_2 = 0;

		sscan( format_1, str_1, s32_1, f32_1 );
		REQUIRE( s32_1 == s32_value );
		REQUIRE( f32_1 == f32_value );

		sscan( format_2, str_2, f32_2, s32_2 );
		REQUIRE( s32_2 == s32_value );
		REQUIRE( f32_2 == f32_value );
	}

	{
		s32 const s32_value_1 = 5505;
		s32 const s32_value_2 = 123;

		CString const str = "5505.123";
		CString const format = "%.%";

		s32 s32_1 = 0;
		s32 s32_2 = 0;

		sscan( format, str, s32_1, s32_2 );
		REQUIRE( s32_value_1 == s32_1 );
		REQUIRE( s32_value_2 == s32_2 );
	}

	da.shutdown();
}