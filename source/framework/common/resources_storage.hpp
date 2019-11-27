/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace con
{
class Resources_Storage final
{
	struct Resource_Info final
	{
		std::string_view const name;
		std::string_view const path;
	};

	using Resources_Info_Vec = std::vector<Resource_Info>;

	struct Resources_Data final
	{
		Resources_Info_Vec const textures, sound_buffers, music, fonts, json_files;
	};

	template <typename TResource>
	struct Resource_Validation_Wrapper final
	{
		TResource resource;
		bool is_valid{ true };
	};

public:
	[[nodiscard]] auto get_texture( std::string const& name ) const      -> sf::Texture const* const;
	[[nodiscard]] auto get_sound_buffer( std::string const& name ) const -> sf::SoundBuffer const* const;
	[[nodiscard]] auto get_music( std::string const& name )              -> sf::Music* const;
	[[nodiscard]] auto get_font( std::string const& name ) const         -> sf::Font const* const;
	[[nodiscard]] auto get_json( std::string const& name ) const         -> nlohmann::json const* const;

	void reload();

	static Resources_Storage& get_instance();

private:
	template <typename TResource>
	using Storage = std::unordered_map<std::string_view, Resource_Validation_Wrapper<TResource>>;

	Storage<sf::Texture>     textures;
	Storage<sf::SoundBuffer> sound_buffers;
	Storage<sf::Music>       music;
	Storage<sf::Font>        fonts;
	Storage<nlohmann::json>  json_files;

	// they call me a madman...
	Resources_Data resources_data{
	#include "game/resources.inl"
	};
};
}

#define G_Resources_Storage con::Resources_Storage::get_instance()