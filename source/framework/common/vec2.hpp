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

	[[nodiscard]] constexpr static Vec2 ZERO()  { return { 0, 0 }; };
	[[nodiscard]] constexpr static Vec2 LEFT()  { return { -1, 0 }; };
	[[nodiscard]] constexpr static Vec2 RIGHT() { return { 1, 0 }; };
	[[nodiscard]] constexpr static Vec2 UP()    { return { 0, -1 }; };
	[[nodiscard]] constexpr static Vec2 DOWN()  { return { 0, 1 }; };

	constexpr Vec2() = default;
	constexpr Vec2( r32 x_, r32 y_ ) : x( x_ ), y( y_ ) {}
	Vec2( sf::Vector2f const& vec ) : x( vec.x ), y( vec.y ) {}

	[[nodiscard]] r32 length() const;
	[[nodiscard]] r32 length_squared() const;
	[[nodiscard]] r32 distance( Vec2 const& second ) const;
	[[nodiscard]] r32 dot( Vec2 const& second ) const;
	[[nodiscard]] r32 cross( Vec2 const& second ) const;
	[[nodiscard]] r32 angle() const;
	void normalize();

	[[nodiscard]] operator sf::Vector2f() const;
	template <typename T>
	[[nodiscard]] operator sf::Vector2<T>() const
	{
		return static_cast<sf::Vector2<T>>( static_cast<sf::Vector2f>( *this ) );
	}

	[[nodiscard]] Vec2 operator-() const;

	[[nodiscard]] Vec2& operator*=( r32 scalar );
	[[nodiscard]] Vec2 operator*( r32 scalar );

	[[nodiscard]] Vec2& operator+=( Vec2 const& rhs );
	[[nodiscard]] Vec2& operator-=( Vec2 const& rhs );
	[[nodiscard]] Vec2 operator+( Vec2 const& rhs );
	[[nodiscard] ]Vec2 operator-( Vec2 const& rhs );

};

using Point = Vec2;
using Size = Vec2;
}