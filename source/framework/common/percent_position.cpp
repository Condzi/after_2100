/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "percent_position.hpp"
#include "assertions.hpp"

namespace con
{
Percent_Position::Percent_Position( r64 x_, r64 y_ )
{
	if ( x_ > 100 or x_ < 0 or
		 y_ > 100 or y_ < 0 ) {
		engine_log_error( "Percent_Position values out of bounds: {}, {}", x_, y_ );
		return;
	}

	x = static_cast<r32>( x_ );
	y = static_cast<r32>( y_ );
}

Percent_Position::operator Vec2() const
{
	return Vec2{ x, y } * 0.01f;
}
}