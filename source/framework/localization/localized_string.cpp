/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "localized_string.hpp"

#include "framework/common/assertions.hpp"

namespace con
{
Localized_String::Localized_String()
{
	locale_language_change_disconnector =
		G_Locale.s_language_change.connect( [this] {
		localized_text = &G_Locale.get_string( locale_name );
		s_update_localized_string.emit();
											} );
}

Localized_String::~Localized_String()
{
	locale_language_change_disconnector();
}

void Localized_String::set_locale_name( std::string const& name )
{
	locale_name = name;
	localized_text = &G_Locale.get_string( name );
}

auto Localized_String::get_localized_text() const -> sf::String const&
{
	return *localized_text;
}
}