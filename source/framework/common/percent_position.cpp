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
	x = cast<r32>( x_ );
	y = cast<r32>( y_ );
}

Percent_Position::operator Vec2() const
{
	return Vec2{ x, y } * 0.01f;
}
}