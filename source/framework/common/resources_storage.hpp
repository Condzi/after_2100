/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>

namespace con
{
class Resources_Storage final
{
	struct Resource_Info final
	{
		std::string name;
		std::string path;

		Resource_Info( std::string const& name_, std::string const& path_ );
	};

	using Resources_Info_Vec = std::vector<Resource_Info>;

	struct Resources_Data final
	{
		Resources_Info_Vec textures, sound_buffers, music, fonts;
	};

	template <typename TResource>
	struct Resource_Validation_Wrapper final
	{
		TResource resource;
		bool is_valid{ true };
	};

public:
	auto get_texture( std::string const& name ) const        -> sf::Texture const* const;
	auto get_sound_buffer( std::string const& name ) const   -> sf::SoundBuffer const* const;
	auto get_music( std::string const& name )                -> sf::Music * const;
	auto get_font( std::string const& name ) const           -> sf::Font const* const;

	void reload();

	static Resources_Storage& get_instance();

private:
	std::unordered_map<std::string, Resource_Validation_Wrapper<sf::Texture>> textures;
	std::unordered_map<std::string, Resource_Validation_Wrapper<sf::SoundBuffer>> sound_buffers;
	std::unordered_map<std::string, Resource_Validation_Wrapper<sf::Music>> music;
	std::unordered_map<std::string, Resource_Validation_Wrapper<sf::Font>> fonts;

	// they call me a madman...
	Resources_Data resources_data{
	#include "game/resources.inl" 
	};
};
}

#define G_Resource_Storage con::Resources_Storage::get_instance()