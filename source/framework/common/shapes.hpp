/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "vec2.hpp"

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
};

struct Circle_Shape final
{
	Point center;
	r32   radious{ 0 };
};

bool RectVsRect( Rectangle_Shape const& a, Rectangle_Shape const& b );
bool RectVsPoint( Rectangle_Shape const& rect, Point const& point );
}