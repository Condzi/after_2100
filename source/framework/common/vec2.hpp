/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include <SFML/System/Vector2.hpp>

#include "framework/common/typedefs.hpp"

namespace con
{
class Vec2 final
{
public:
	union
	{
		r32 x{ 0 };
		r32 width;
	};
	union
	{
		r32 y{ 0 };
		r32 height;
	};

	[[nodiscard]] constexpr static Vec2 ZERO()  { return {  0,  0 }; };
	[[nodiscard]] constexpr static Vec2 LEFT()  { return { -1,  0 }; };
	[[nodiscard]] constexpr static Vec2 RIGHT() { return {  1,  0 }; };
	[[nodiscard]] constexpr static Vec2 UP()    { return {  0, -1 }; };
	[[nodiscard]] constexpr static Vec2 DOWN()  { return {  0,  1 }; };

	constexpr Vec2() = default;
	constexpr Vec2( r32 x_, r32 y_ ) : x( x_ ), y( y_ ) {}
	template <typename T>
	constexpr Vec2( T x_, T y_ ) : x( static_cast<r32>( x_ ) ), y( static_cast<r32>( y_ ) ) {}
	Vec2( sf::Vector2f const& vec ) : x( vec.x ), y( vec.y ) {}
	template <typename T>
	Vec2( sf::Vector2<T> const& vec ) : Vec2( static_cast<sf::Vector2f>( vec ) ) {}

	[[nodiscard]] r32 length() const;
	[[nodiscard]] r32 length_squared() const;
	[[nodiscard]] r32 distance( Vec2 const& second ) const;
	[[nodiscard]] r32 distance_squared( Vec2 const& second ) const;
	[[nodiscard]] r32 dot( Vec2 const& second ) const;
	[[nodiscard]] r32 cross( Vec2 const& second ) const;
	[[nodiscard]] r32 angle() const;
	[[nodiscard]] auto normalize() -> Vec2 &;

	[[nodiscard]] operator sf::Vector2f() const;
	template <typename T>
	[[nodiscard]] operator sf::Vector2<T>() const
	{
		return static_cast<sf::Vector2<T>>( static_cast<sf::Vector2f>( *this ) );
	}

	[[nodiscard]] Vec2 operator-() const;

	Vec2& operator*=( r32 scalar );
	[[nodiscard]] Vec2 operator*( r32 scalar ) const;

	Vec2& operator+=( Vec2 const& rhs );
	Vec2& operator-=( Vec2 const& rhs );
	Vec2& operator*=( Vec2 const& rhs );
	[[nodiscard]] Vec2 operator+( Vec2 const& rhs ) const;
	[[nodiscard]] Vec2 operator-( Vec2 const& rhs ) const;
	[[nodiscard]] Vec2 operator*( Vec2 const& rhs ) const;

	[[nodiscard]] bool operator==( Vec2 const& rhs ) const;
	[[nodiscard]] bool operator>( Vec2 const& rhs ) const;
	[[nodiscard]] bool operator<( Vec2 const& rhs ) const;
};

Vec2 truncate( Vec2 const& in, r32 max );

using Point = Vec2;
using Size2 = Vec2;
}