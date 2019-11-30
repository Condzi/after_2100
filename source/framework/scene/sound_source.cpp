/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/debug_flags.hpp"
#include "framework/common/resources_storage.hpp"

#include "sound_source.hpp"

namespace con
{
Sound_Source::Sound_Source()
{
	bond_disconnector( s_paused.connect( [this]( bool status ) {
		if ( status is true and sound.getStatus() is sf::Music::Playing )
			sound.pause();
		else if ( status is false and sound.getStatus() is sf::Music::Paused )
			sound.play();
	} ) );
}

auto Sound_Source::is_looped() const -> bool
{
	return sound.getLoop();
}

auto Sound_Source::is_playing() const -> bool
{
	return sound.getStatus() is sf::Sound::Playing;
}

auto Sound_Source::is_paused() const -> bool
{
	return sound.getStatus() is sf::Sound::Paused;
}

auto Sound_Source::is_stopped() const -> bool
{
	return sound.getStatus() is sf::Sound::Stopped;
}

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
	report_warning_if( buffer is nullptr )
	{
		return;
	}

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
	static constant& draw_audio_sources = G_Debug_Flags.get( "draw_audio_sources" );

	if ( draw_audio_sources is false )
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