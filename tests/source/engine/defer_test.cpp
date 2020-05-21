#include <catch/catch.hpp>

#include <engine/typedefs.hpp>

TEST_CASE( "defer", "engine" )
{
	int some_value = 0;
	{
		defer{ some_value = 999; };
		REQUIRE( some_value == 0 );

		some_value = 1;
	}

	REQUIRE( some_value == 999 );
}