/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "node.hpp"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace con
{
class Sound_Source final :
	public Node
{
	CLASS_DEF( Sound_Source );
	
public:
	sf::Color shape_color{ sf::Color::Green };
	s32       layer{ 10 };

	void set_attenuation( r32 attenuation );
	void set_minimum_distance( r32 distance );
	void set_loop( bool value );
	void set_sound_buffer_from_pointer( sf::SoundBuffer const* buffer );
	void set_sound_buffer_from_name( std::string const& name );

	void update( r32 dt ) override;
	void draw( Drawing_Set& set ) override;

	void play();
	void stop();
	void pause();

private:
	sf::Sound sound;

	sf::CircleShape debug_shape{ 10.0px };
};
}