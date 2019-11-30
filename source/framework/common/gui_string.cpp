/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "gui_string.hpp"

namespace con
{
GUI_String::GUI_String()
{
	update_localized_string_disconnector =
		localized_string.s_update_localized_string.connect( [this] {
		if ( use_locale ) {
			string = localized_string.get_localized_text();
			s_locale_changed.emit();
		}
	} );
}

GUI_String::~GUI_String()
{
	update_localized_string_disconnector();
}

void GUI_String::set_locale_name( std::string const& name )
{
	localized_string.set_locale_name( name );
	string = localized_string.get_localized_text();
	use_locale = true;
}

void GUI_String::set_ascii( std::string const& str )
{
	string = str;
	use_locale = false;
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