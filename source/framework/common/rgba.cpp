/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/rgba.hpp"

namespace con
{
RGBA::operator sf::Color() const
{
	return sf::Color( r, g, b, a );
}

}