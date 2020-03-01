/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "assertions.hpp"
#include "shapes.hpp"

namespace con
{
bool rect_vs_rect( Rectangle_Shape const& a, Rectangle_Shape const& b )
{
	return sf::FloatRect( a.position, a.size ).intersects( { b.position, b.size } );
}

bool rect_vs_point( Rectangle_Shape const& rect, Point const& point )
{
	return sf::FloatRect( rect.position, rect.size ).contains( point );
}
bool circle_vs_circle( Circle_Shape const& a, Circle_Shape const& b )
{
	constant radious_sum = a.radius + b.radius;
	constant x_sum = a.center.x + b.center.x;
	constant y_sum = a.center.y + b.center.y;

	return radious_sum*radious_sum < x_sum*x_sum + y_sum*y_sum;
}

bool circle_vs_rect( Circle_Shape const& circle, Rectangle_Shape const& rect )
{
	// Source: https://github.com/RandyGaul/cute_headers/blob/master/cute_c2.h#L1184

	Vec2 const L{ std::clamp( circle.center.x, rect.position.x, rect.position.x + rect.size.width ),
				std::clamp( circle.center.y, rect.position.y, rect.position.y + rect.size.height ) };
	Vec2 const ab{ circle.center.x - L.x, circle.center.y - L.y };

	r32 const d2 = ab.dot( ab );
	r32 const r2 = circle.radius * circle.radius;

	return d2 < r2;
}

bool circle_vs_point( Circle_Shape const& circle, Point const& point )
{
	constant distance_squared = circle.center.distance_squared( point );
	constant radious_squared = circle.radius * circle.radius;

	return distance_squared <= radious_squared;
}


bool shape_vs_shape( Shape_Variant const& a, Shape_Variant const& b )
{
	return std::visit( visitor_overload{
		[]( Rectangle_Shape const& a, Rectangle_Shape const& b ) {
			return rect_vs_rect( a, b );
		},
		[]( Circle_Shape const& a, Circle_Shape const& b ) {
			return circle_vs_circle( a, b );
		},
		[]( Circle_Shape const& a, Rectangle_Shape const& b ) {
			return circle_vs_rect( a, b );
		},
		[]( Rectangle_Shape const& a, Circle_Shape const& b ) {
			return circle_vs_rect( b, a );
		} }, a, b );
}
}