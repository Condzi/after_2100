#include <catch/catch.hpp>
#include <engine/sprint.hpp>

#include <sstream>
#include <string>

TEST_CASE( "sprint", "engine" )
{
	using namespace con;

	SECTION( "f32" )
	{
		Temporary_Allocator ta;
		Default_Allocator da;
		Context.default_allocator = &da;
		Context.temporary_storage_allocator = &ta;
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


		Context.temporary_storage_allocator = nullptr;
	}


}