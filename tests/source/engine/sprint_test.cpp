#include <catch/catch.hpp>
#include <engine/sprint.hpp>

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
		Context.temporary_storage_allocator = &ta;
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
		Context.temporary_storage_allocator = nullptr;
		da.shutdown();
	}

	SECTION( "s32" )
	{
		Temporary_Allocator ta;
		Default_Allocator da;
		Context.default_allocator = &da;
		Context.temporary_storage_allocator = &ta;
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
		Context.temporary_storage_allocator = nullptr;
		da.shutdown();
	}

	SECTION( "sprint" )
	{
		Temporary_Allocator ta;
		Default_Allocator da;
		Context.default_allocator = &da;
		Context.temporary_storage_allocator = &ta;
		da.initialize();
		ta.initialize();

		constant str = sprint( "value_s32 = %\nvalue_f32 = %\nvalue_str = %.\n"_cs, 1337, 21.37, "Oh, hi Mark!"_cs );

		for ( s32 i = 0; i < str.size; ++i ) {
			putchar( str.data[i] );
		}

		printf( "Memory used: %i / %i bytes.\n", ta.get_mark(), CON_TEMPORARY_STORAGE_RESERVED_MEMORY );

		Context.default_allocator = nullptr;
		Context.temporary_storage_allocator = nullptr;
		da.shutdown();
	}
}