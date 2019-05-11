/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "stl_extensions.hpp"

namespace con
{
bool string_begins_with( std::string const& string, std::string const& prefix )
{
	return string.substr( prefix.size() ) == prefix;
}
}