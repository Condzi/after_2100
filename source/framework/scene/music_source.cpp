/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "music_source.hpp"
#include "framework/common/debug_flags.hpp"
#include "framework/common/resources_storage.hpp"

namespace con
{
void Music_Source::set_attenuation( r32 attenuation )
{
	report_error_if( attenuation < 0 or attenuation > 100 )
	{
		return;
	}

	music->setAttenuation( attenuation );
}

void Music_Source::set_minimum_distance( r32 distance )
{
	report_error_if( distance < 1 )
	{
		return;
	}

	music->setMinDistance( distance );
}

void Music_Source::set_loop( bool value )
{
	music->setLoop( value );
}

void Music_Source::set_relative_to_audio_listener( bool value )
{
	music->setRelativeToListener( value );
}

void Music_Source::set_music_from_name( std::string const& name )
{
	music = G_Resources_Storage.get_music( name );
}

void Music_Source::update( r32 dt )
{
	unused( dt );

	music->setPosition( get_global_position().x, get_global_position().y, 0 );

	if ( constant current_status = music->getStatus(); current_status != previous_status ) {
		previous_status = current_status;

		if ( current_status == sf::Music::Status::Playing )
			s_music_play.emit();
		else if ( current_status == sf::Music::Status::Paused )
			s_music_pause.emit();
		else if ( current_status == sf::Music::Status::Stopped )
			s_music_stop.emit();
	}
}

void Music_Source::draw( Drawing_Set& set )
{
	if ( not G_Debug_Flags.get( "draw_audio_sources" ) )
		return;

	debug_shape.setPosition( get_global_position() );
	debug_shape.setOutlineColor( shape_color );
	debug_shape.setOutlineThickness( 1.0px );
	debug_shape.setFillColor( sf::Color::Transparent );

	set.add_drawable( debug_shape, layer );
}

void Music_Source::play()
{
	music->play();
}

void Music_Source::stop()
{
	music->stop();
}

void Music_Source::pause()
{
	music->pause();
}
}