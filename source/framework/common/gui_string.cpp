/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "gui_string.hpp"

namespace con
{
void GUI_String::set_locale_name( std::string const& name )
{
	localized_string.set_locale_name( name );
	string = localized_string.get_localized_text();
}

void GUI_String::set_ascii( std::string const& str )
{
	string = str;
}

void GUI_String::set( sf::String const& str )
{
	string = str;
}

auto GUI_String::get_string() const -> sf::String const&
{
	return string;
}
}