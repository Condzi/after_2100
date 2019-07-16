/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "localized_string.hpp"

#include "framework/common/assertions.hpp"

namespace con
{
void Localized_String::set_locale_name( std::string const& name )
{
	localized_text = &G_Locale.get_string( name );
}

auto Localized_String::get_localized_text() const -> sf::String const&
{
	return *localized_text;
}
}