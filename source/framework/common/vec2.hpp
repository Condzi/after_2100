/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include <SFML/System/Vector2.hpp>

#include "pch.hpp"

namespace con
{
class Vec2 final
{
public:
	r32 x{ 0 };
	r32 y{ 0 };

	constexpr static Vec2 ZERO()  { return { 0, 0 }; };
	constexpr static Vec2 LEFT()  { return { -1, 0 }; };
	constexpr static Vec2 RIGHT() { return { 1, 0 }; };
	constexpr static Vec2 UP()    { return { 0, -1 }; };
	constexpr static Vec2 DOWN()  { return { 0, 1 }; };

	constexpr Vec2() = default;
	constexpr Vec2( r32 x_, r32 y_ ) : x( x_ ), y( y_ ) {}
	Vec2( sf::Vector2f const& vec ) : x( vec.x ), y( vec.y ) {}

	r32 length() const;
	r32 length_squared() const;
	r32 distance( Vec2 const& second ) const;
	void normalize();

	operator sf::Vector2f() const;
	template <typename T>
	operator sf::Vector2<T>() const
	{
		return static_cast<sf::Vector2<T>>( static_cast<sf::Vector2f>( *this ) );
	}

	Vec2 operator-() const;

	Vec2& operator*=( r32 scalar );
	Vec2 operator*( r32 scalar );

	Vec2& operator+=( Vec2 const& rhs );
	Vec2& operator-=( Vec2 const& rhs );
	Vec2 operator+( Vec2 const& rhs );
	Vec2 operator-( Vec2 const& rhs );

	friend std::ostream& operator<<( std::ostream& out, Vec2 const& vec );
};
}