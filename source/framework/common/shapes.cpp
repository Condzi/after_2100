/*
	Conrad 'Condzi' Kubacki 2019
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
	constant radious_sum = a.radious + b.radious;
	constant x_sum = a.center.x + b.center.x;
	constant y_sum = a.center.y + b.center.y;

	return radious_sum*radious_sum < x_sum*x_sum + y_sum*y_sum;
}

bool circle_vs_rect( Circle_Shape const& circle, Rectangle_Shape const& rect )
{
	// AABB Verticies, clockwise
	// A__B
	// |  |
	// D__C
	constant A = rect.position;
	constant B = A + Vec2{ rect.size.x, 0 };
	constant C = A + rect.size;
	constant D = A + Vec2{ 0, rect.size.y };

	return rect_vs_point( rect, circle.center ) or
		circle_vs_point( circle, A ) or
		circle_vs_point( circle, B ) or
		circle_vs_point( circle, C ) or
		circle_vs_point( circle, D );
}

bool circle_vs_point( Circle_Shape const& circle, Point const& point )
{
	constant distance_squared = circle.center.distance_squared( point );
	constant radious_squared = circle.radious * circle.radious;

	return distance_squared <= radious_squared;
}

bool shape_vs_shape( Shape_Variant const& a, Shape_Variant const& b )
{
	struct Shape_Cases_Visitor final
	{
		bool operator()( Rectangle_Shape const& a, Rectangle_Shape const& b ) const
		{
			return rect_vs_rect( a, b );
		}

		bool operator()( Circle_Shape const& a, Circle_Shape const& b ) const
		{
			return circle_vs_circle( a, b );
		}

		bool operator()( Circle_Shape const& a, Rectangle_Shape const& b ) const
		{
			return circle_vs_rect( a, b );
		}

		bool operator()( Rectangle_Shape const& a, Circle_Shape const& b ) const
		{
			return circle_vs_rect( b, a );
		}

	};

	return std::visit( Shape_Cases_Visitor{}, a, b );
}

}