/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/assertions.hpp"
#include "framework/common/vec2.hpp"

namespace con
{
r32 Vec2::length() const
{
	return std::sqrtf( x*x + y*y );
}

r32 Vec2::length_squared() const
{
	return x*x + y*y;
}

r32 Vec2::distance( Vec2 const& second ) const
{
	return std::sqrtf( distance_squared( second ) );
}

r32 Vec2::distance_squared( Vec2 const& second ) const
{
	constant x_diff_sqr{ ( x - second.x ) * ( x - second.x ) };
	constant y_diff_sqr{ ( y - second.y ) * ( y - second.y ) };

	return x_diff_sqr + y_diff_sqr;
}

r32 Vec2::dot( Vec2 const& second ) const
{
	return x * second.x + y * second.y;
}

r32 Vec2::cross( Vec2 const& second ) const
{
	return x * second.y - x * second.y;
}

r32 Vec2::angle() const
{
	return std::atan2f( y, x );
}

auto Vec2::normalize() -> Vec2&
{
	r32 const len{ length() };
	if ( len is 0 ) {
		*this = Vec2::Zero();
	} else {
		r32 inv_length{ 1 / len };
		*this *= inv_length;
	}

	return *this;
}

Vec2::operator sf::Vector2f() const
{
	return sf::Vector2f{ x, y };
}

Vec2 Vec2::operator-() const
{
	return Vec2{ -x, -y };
}

Vec2& Vec2::operator*=( r32 scalar )
{
	x *= scalar;
	y *= scalar;

	return *this;
}

Vec2 Vec2::operator*( r32 scalar ) const
{
	return Vec2{ x, y } *= scalar;
}

Vec2& Vec2::operator+=( Vec2 const& rhs )
{
	x += rhs.x;
	y += rhs.y;

	return *this;
}

Vec2& Vec2::operator-=( Vec2 const& rhs )
{
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

Vec2& Vec2::operator*=( Vec2 const& rhs )
{
	x *= rhs.x;
	y *= rhs.y;

	return *this;
}

Vec2 Vec2::operator+( Vec2 const& rhs ) const
{
	return Vec2{ x,y } += rhs;
}

Vec2 Vec2::operator-( Vec2 const& rhs ) const
{
	return Vec2{ x,y } -= rhs;
}
Vec2 Vec2::operator*( Vec2 const& rhs ) const
{
	return Vec2( x, y ) *= rhs;
}

bool Vec2::operator==( Vec2 const& rhs ) const
{
	return x is rhs.x and y is rhs.y;
}

bool Vec2::operator>( Vec2 const& rhs ) const
{
	return x > rhs.x and y > rhs.y;
}

bool Vec2::operator<( Vec2 const& rhs ) const
{
	return x < rhs.x and y < rhs.y;
}

Vec2 truncate( Vec2 vec, r32 max )
{
	if ( vec.x > max )
		vec.x = max;

	if ( vec.y > max )
		vec.y = max;

	return vec;
}
}
