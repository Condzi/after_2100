/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"
#include "shapes.hpp"

namespace con
{
bool RectVsRect( Rectangle_Shape const& a, Rectangle_Shape const& b )
{
	return sf::FloatRect( a.position, a.size ).intersects( { b.position, b.size } );
}

bool RectVsPoint( Rectangle_Shape const& rect, Point const& point )
{
	return sf::FloatRect( rect.position, rect.size ).contains( point );
}
}