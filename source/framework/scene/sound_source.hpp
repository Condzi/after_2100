/*
	Conrad 'Condzi' Kubacki 2020
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
	sf::Color shape_color{ sf::Color::Magenta };
	s32       layer{ 50 };

	Sound_Source();

	[[nodiscard]] auto is_looped() const  -> bool;
	[[nodiscard]] auto is_playing() const -> bool;
	[[nodiscard]] auto is_paused() const  -> bool;
	[[nodiscard]] auto is_stopped() const -> bool;

	void set_attenuation( r32 attenuation );
	void set_minimum_distance( r32 distance );
	void set_loop( bool value );
	void set_relative_to_audio_listener( bool value );
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