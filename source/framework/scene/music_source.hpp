/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "node.hpp"

#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace con
{
class Music_Source final :
	public Node
{
	CLASS_DEF( Music_Source );

public:
	sf::Color shape_color{ sf::Color::Yellow };
	s32       layer{ 10 };

	void set_attenuation( r32 attenuation );
	void set_minimum_distance( r32 distance );
	void set_loop( bool value );
	void set_relative_to_audio_listener( bool value );
	void set_music_from_name( std::string const& name );

	void update( r32 dt ) override;
	void draw( Drawing_Set& set ) override;

	void play();
	void stop();
	void pause();

private:
	sf::Music* music;

	sf::CircleShape debug_shape{ 10.0px };
};
}