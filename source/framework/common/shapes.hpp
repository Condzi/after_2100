/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "vec2.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace con
{
struct Rectangle_Shape final
{
	Point position;
	Size2 size;

	Rectangle_Shape() = default;
	Rectangle_Shape( Point const& position_, Size2 const& size_ ) : position( position_ ), size( size_ ) {}
	template <typename T>
	Rectangle_Shape( sf::Rect<T> const& rect ) :
		position( static_cast<r32>( rect.left ), static_cast<r32>( rect.top ) ),
		size( static_cast<r32>( rect.width ), static_cast<r32>( rect.height ) )
	{}

	template <typename T>
	[[nodiscard]] operator sf::Rect<T>() const
	{
		return sf::Rect<T>{ static_cast<T>( position.x ), static_cast<T>( position.y ), static_cast<T>( size.width ), static_cast<T>( size.height ) };
	}
};

struct Circle_Shape final
{
	Point center;
	r32   radious{ 0 };
};

using Shape_Variant = std::variant<Rectangle_Shape, Circle_Shape>;

bool rect_vs_rect( Rectangle_Shape const& a, Rectangle_Shape const& b );
bool rect_vs_point( Rectangle_Shape const& rect, Point const& point );
bool circle_vs_circle( Circle_Shape const& a, Circle_Shape const& b );
bool circle_vs_rect( Circle_Shape const& circle, Rectangle_Shape const& rect );
bool circle_vs_point( Circle_Shape const& circle, Point const& point );

bool shape_vs_shape( Shape_Variant const& a, Shape_Variant const& b );
}