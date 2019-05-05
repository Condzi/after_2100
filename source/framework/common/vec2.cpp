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
	r32 const x_diff_sqr{ ( x - second.x ) * ( x - second.x ) };
	r32 const y_diff_sqr{ ( y - second.y ) * ( y - second.y ) };

	return std::sqrtf( x_diff_sqr + y_diff_sqr );
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

void Vec2::normalize()
{
	r32 const len{ length() };
	report_warning_if( len == 0 )
	{
		x = y = 0;
	} else {
		r32 inv_length{ 1 / len };
		x *= inv_length;
		y *= inv_length;
	}
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

Vec2 Vec2::operator*( r32 scalar )
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

Vec2 Vec2::operator+( Vec2 const& rhs )
{
	return Vec2{ x,y } += rhs;
}

Vec2 Vec2::operator-( Vec2 const& rhs )
{
	return Vec2{ x,y } -= rhs;
}

std::ostream& operator<<( std::ostream& out, Vec2 const& vec )
{
	out << '[' << vec.x << ", " << vec.y << ']';

	return out;
}
}
