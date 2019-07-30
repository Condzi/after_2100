/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "sound_source.hpp"

#include "framework/common/debug_flags.hpp"
#include "framework/common/resources_storage.hpp"

namespace con
{
void Sound_Source::set_attenuation( r32 attenuation )
{
	report_error_if( attenuation < 0 or attenuation > 100 )
	{
		return;
	}

	sound.setAttenuation( attenuation );
}

void Sound_Source::set_minimum_distance( r32 distance )
{
	report_error_if( distance < 1 )
	{
		return;
	}

	sound.setMinDistance( distance );
}

void Sound_Source::set_loop( bool value )
{
	sound.setLoop( value );
}

void Sound_Source::set_relative_to_audio_listener( bool value )
{
	sound.setRelativeToListener( value );
}

void Sound_Source::set_sound_buffer_from_pointer( sf::SoundBuffer const* buffer )
{
	sound.setBuffer( *buffer );
}

void Sound_Source::set_sound_buffer_from_name( std::string const& name )
{
	set_sound_buffer_from_pointer( G_Resources_Storage.get_sound_buffer( name ) );
}

void Sound_Source::update( r32 dt )
{
	unused( dt );

	sound.setPosition( get_global_position().x, get_global_position().y, 0 );
}

void Sound_Source::draw( Drawing_Set& set )
{
	if ( not G_Debug_Flags.draw_audio_sources )
		return;

	debug_shape.setPosition( get_global_position() );
	debug_shape.setOutlineColor( shape_color );
	debug_shape.setOutlineThickness( 1.0px );
	debug_shape.setFillColor( sf::Color::Transparent );

	set.add_drawable( debug_shape, layer );
}

void Sound_Source::play()
{
	sound.play();
}

void Sound_Source::stop()
{
	sound.stop();
}

void Sound_Source::pause()
{
	sound.pause();
}

}