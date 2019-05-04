/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

#include <SFML/Graphics/Color.hpp>

namespace con
{
class RGBA final
{
public:
	static constexpr RGBA BLACK()       { return { 0,0,0 }; };
	static constexpr RGBA WHITE()       { return { 255,255,255 }; };
	static constexpr RGBA RED()         { return { 255,0,0 }; };
	static constexpr RGBA GREEN()       { return { 0,255,0 }; };
	static constexpr RGBA BLUE()        { return { 0,0,255 }; };
	static constexpr RGBA YELLOW()      { return { 255,255,0 }; };
	static constexpr RGBA MAGENTA()     { return { 255,0,255 }; };
	static constexpr RGBA CYAN()        { return { 0,255,255 }; };
	static constexpr RGBA TRANSPARENT() { return { 0,0,0,0 }; };

	u8 r{ 0 };
	u8 g{ 0 };
	u8 b{ 0 };
	u8 a{ 255 };

	constexpr RGBA() = default;
	constexpr RGBA( u8 r_, u8 g_, u8 b_, u8 a_ = 255 ) : r( r_ ), g( g_ ), b( b_ ), a( a_ ) {}
	RGBA( sf::Color const& color ) : r( color.r ), g( color.g ), b( color.b ), a( color.a ) {}

	operator sf::Color() const;
};
}