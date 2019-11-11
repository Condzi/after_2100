/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/stl_extensions.hpp"

#include "locale.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <SFML/System/Clock.hpp>

namespace con::priv
{
void Locale::set_current_language( std::string const& language )
{
	if ( is_language_avaible( language ) returned false )
		engine_log_error( "'{}' language is not avaible.", language );
	else
		current_language = language;
}

auto Locale::get_fallback_string() const -> sf::String const&
{
	return fallback_string;
}

auto Locale::get_current_language() const -> std::string const&
{
	return current_language;
}

auto Locale::get_avaible_languages() const -> std::vector<std::string> const&
{
	return avaible_languages;
}

auto Locale::get_string( std::string const& name ) -> sf::String const&
{
	if ( auto it = strings.find( name ); it is strings.end() ) {
		engine_log_error( "Can't find string '{}'.", name );
		return fallback_string;
	} else
		return it->second;
}

void Locale::reload()
{
	using namespace nlohmann;

	engine_log_info( "Reloading locale." );
	sf::Clock timer;

	json raw_data;
	std::ifstream input_file{ PATH_TO_LOC_FILE };

	if ( !input_file ) {
		engine_log_error( "Can't load localization file from '{}'.", PATH_TO_LOC_FILE );
		return;
	}

	try {
		input_file >> raw_data;

		for ( constant& record : raw_data.items() ) {
			constant& key = record.key();
			constant& value = record.value();

			if ( string_begins_with( key, "#loc" ) ) {
				sf::String string_to_add;
				constant ansi_string = value.at( current_language ).get<std::string>();
				string_to_add = sf::String::fromUtf8( ansi_string.cbegin(), ansi_string.cend() );

				strings[key] = string_to_add;
			}
		}
	}
	catch ( std::exception const& ex ) {
		engine_log_error( "Failed to load loc from json file. Exception: '{}'.", ex.what() );
	}

	engine_log_info( "Locale reload end. It took {0:.3f}s.", timer.getElapsedTime().asSeconds() );
}

Locale& Locale::get_instance()
{
	static Locale* instance = new Locale;

	return *instance;
}

bool Locale::is_language_avaible( std::string const& lang )
{
	return find( avaible_languages, lang ).found;
}
}