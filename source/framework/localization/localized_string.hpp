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
	Signal<> s_update_localized_string;

	Localized_String();
	~Localized_String();

	void set_locale_name( std::string const& name );

	[[nodiscard]] auto get_localized_text() const -> sf::String const&;

private:
	Signal<>::Disconnector locale_language_change_disconnector;

	sf::String locale_name;
	sf::String const* localized_text{ &G_Locale.get_fallback_string() };
};
}