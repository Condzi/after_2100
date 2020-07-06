#include <testing_utilities.hpp>

#include <engine/time.hpp>

TEST_CASE( "time conversions", "engine" )
{
	using namespace con;
	Time_Period const a{ Time::from_microseconds( 1500 ) };
	Time_Period const b{ Time::from_microseconds( 1000 ) };
	Time_Period const c{ a-b };

	REQUIRE( Time::to_microseconds( a ) == 1500 );
	REQUIRE( Time::to_milliseconds( a ) == 1 );
	REQUIRE( Time::to_seconds( a )		== 0.0015f );

	REQUIRE( Time::to_microseconds( b ) == 1000 );
	REQUIRE( Time::to_milliseconds( b ) == 1 );
	REQUIRE( Time::to_seconds( b )		== 0.001f );

	
	REQUIRE( Time::to_microseconds( c ) == 500 );
	REQUIRE( Time::to_milliseconds( c ) == 0 );
	REQUIRE( Time::to_seconds( c )		== 0.0005f );
}