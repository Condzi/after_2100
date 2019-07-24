/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include <SFML/System/String.hpp>

#include "framework/localization/localized_string.hpp"
#include "assertions.hpp"

namespace con
{
class GUI_String final
{
public:
	void set_locale_name( std::string const& name );
	
	// Locale can have paremeters like: "Hello, {0}!" => "Hello, Conrad!"
	template <typename ...TArgs>
	void update_locale_parameters( TArgs ...args );

	void set_ascii( std::string const& str );

	[[nodiscard]] auto get_string() const -> sf::String const&;

private:
	// It's just a pointer so don't bother with union or variant.
	Localized_String localized_string;
	// Ascii string or formated localized string.
	sf::String string;
};
}

#include "gui_string.inl"