/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "locale.hpp"

namespace con
{
// Just a reference to value in G_Locale
class Localized_String final
{
public:
	void set_locale_name( std::string const& name );

	[[nodiscard]] auto get_localized_text() const -> sf::String const&;

private:
	sf::String const* localized_text{ &G_Locale.get_fallback_string() };
};
}