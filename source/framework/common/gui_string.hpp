/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "assertions.hpp"

#include "framework/localization/localized_string.hpp"

#include <SFML/System/String.hpp>

namespace con
{
class GUI_String final
{
public:
	Signal<> s_locale_changed;

	GUI_String();
	~GUI_String();

	void set_locale_name( std::string const& name );

	// Locale can have paremeters like: "Hello, {0}!" => "Hello, Conrad!"
	template <typename ...TArgs>
	void update_locale_parameters( TArgs ...args );

	void set_ascii( std::string const& str );
	void set( sf::String const& str );

	[[nodiscard]] auto get_string() const -> sf::String const&;

private:
	Signal<>::Disconnector update_localized_string_disconnector;

	bool use_locale{ false };
	// It's just a pointer so don't bother with union or variant.
	Localized_String localized_string;
	// Ascii string or formated localized string.
	sf::String string;
};
}

#include "gui_string.inl"