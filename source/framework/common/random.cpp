/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "random.hpp"

namespace con
{
std::mt19937 rng{ static_cast<u32>( std::chrono::high_resolution_clock::now().time_since_epoch().count() ) };

s32 random_int( s32 min, s32 max )
{
	if ( min > max ) {
		engine_log_error( "Can't randomize. min > max ({} > {})", min, max );
		return min;
	}

	std::uniform_int_distribution dist( min, max );

	return dist( rng );
}

r32 random_real( r32 min, r32 max )
{
	if ( min > max ) {
		engine_log_error( "Can't randomize. min > max ({} > {})", min, max );
		return min;
	}

	std::uniform_real_distribution dist( min, max );

	return dist( rng );
}
}