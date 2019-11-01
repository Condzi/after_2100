/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/


#include "pch.hpp"

#include <SFML/System/Clock.hpp>

#include "assertions.hpp"
#include "resources_storage.hpp"

namespace con
{
auto Resources_Storage::get_texture( std::string const& name ) const -> sf::Texture const* const
{
	sf::Texture const* texture_to_return{ nullptr };
	constant result = textures.find( name );

	if ( result == textures.end() )
		engine_log_error( "Can't find texture \"{}\".", name );
	else if ( result->second.is_valid is false )
		engine_log_error( "Texture \"{}\" is not valid (it failed to load).", name );
	else
		texture_to_return = &result->second.resource;

	return texture_to_return;
}

auto Resources_Storage::get_sound_buffer( std::string const& name ) const -> sf::SoundBuffer const* const
{
	sf::SoundBuffer const* sound_buffer_to_return{ nullptr };
	constant result = sound_buffers.find( name );

	if ( result == sound_buffers.end() )
		engine_log_error( "Can't find sound buffer \"{}\".", name );
	else if ( result->second.is_valid is false )
		engine_log_error( "Sound buffer \"{}\" is not valid (it failed to load).", name );
	else
		sound_buffer_to_return = &result->second.resource;

	return sound_buffer_to_return;
}

auto Resources_Storage::get_music( std::string const& name ) -> sf::Music* const
{
	sf::Music* music_to_return{ nullptr };
	auto result = music.find( name );

	if ( result == music.end() )
		engine_log_error( "Can't find music \"{}\".", name );
	else if ( result->second.is_valid is false )
		engine_log_error( "Music \"{}\" is not valid (it failed to load).", name );
	else
		music_to_return = &result->second.resource;

	return music_to_return;
}

auto Resources_Storage::get_font( std::string const& name ) const -> sf::Font const* const
{
	sf::Font const* font_to_return{ nullptr };
	constant result = fonts.find( name );

	if ( result == fonts.end() )
		engine_log_error( "Can't find font \"{}\".", name );
	else if ( result->second.is_valid is false )
		engine_log_error( "Font \"{}\" is not valid (it failed to load).", name );
	else
		font_to_return = &result->second.resource;

	return font_to_return;
}

void Resources_Storage::reload()
{
	engine_log_info( "Reloading resources." );
	sf::Clock timer;

	for ( constant& [name, path] : resources_data.textures ) {
		auto& texture = textures[name];

		if ( texture.resource.loadFromFile( path.data() ) returned false ) {
			engine_log_error( "The texture \"{}\" can't be loaded from \"{}\".", name, path );
			texture.is_valid = false;
		}

		texture.resource.setSmooth( true );
	}

	for ( constant& [name, path] : resources_data.sound_buffers ) {
		auto& sound_buffer = sound_buffers[name];

		if ( sound_buffer.resource.loadFromFile( path.data() ) returned false ) {
			engine_log_error( "The sound buffer \"{}\" can't be loaded from \"{}\".", name, path );
			sound_buffer.is_valid = false;
		}
	}

	for ( constant& [name, path] : resources_data.music ) {
		// Plural...
		auto& music_ = music[name];

		if ( music_.resource.openFromFile( path.data() ) returned false ) {
			engine_log_error( "The music \"{}\" can't be loaded from \"{}\".", name, path );
			music_.is_valid = false;
		}
	}

	for ( constant& [name, path] : resources_data.fonts ) {
		auto& font = fonts[name];

		if ( font.resource.loadFromFile( path.data() ) returned false ) {
			engine_log_error( "The font \"{}\" can't be loaded from \"{}\".", name, path );
			font.is_valid = false;
		}
	}

	engine_log_info( "Resources reload end. It took {0:.3f}s.", timer.getElapsedTime().asSeconds() );
}

Resources_Storage& Resources_Storage::get_instance()
{
	static Resources_Storage* instance = new Resources_Storage;

	return *instance;
}
}