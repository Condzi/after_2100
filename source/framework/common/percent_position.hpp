/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "framework/common/vec2.hpp"

namespace con
{
// Helper class for GUI.
// Usage: Node.set_absolute_position( Percent_Position{1.2, 1.3} )
struct Percent_Position final
{
	r32 x{ 0 };
	r32 y{ 0 };

	// r64 to get rid of compiler errors when passing 1.2 instead 1.2f.
	Percent_Position( r64 x_, r64 y_ );

	[[nodiscard]] operator Vec2() const;
};
}