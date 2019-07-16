/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include <SFML/System/String.hpp>

namespace con::priv
{
class Locale final
{
public:
	static constexpr const char* PATH_TO_LOC_FILE = "resources/loc.json";

	void set_current_language( std::string const& language );

	[[nodiscard]] auto get_fallback_string() const           -> sf::String const&;
	[[nodiscard]] auto get_current_language() const          -> std::string const&;
	[[nodiscard]] auto get_avaible_languages() const         -> std::vector<std::string> const&;
	[[nodiscard]] auto get_string( std::string const& name ) -> sf::String const&;

	void reload();

	static Locale& get_instance();

private:
	// @ToDo: Set from save / config file so the player doesn't need to select it 
	// in every session.
	std::string              current_language{ "pl" };
	std::vector<std::string> avaible_languages;

	sf::String                                  fallback_string{ "#no_loc" };
	std::unordered_map<std::string, sf::String> strings{ 100 }; // reserving space for 100 pairs

	bool is_language_avaible( std::string const& lang );
};
}

#define G_Locale con::priv::Locale::get_instance()