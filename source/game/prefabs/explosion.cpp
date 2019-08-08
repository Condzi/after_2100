/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "explosion.hpp"
#include "framework/scene/sound_source.hpp"

Explosion::Explosion()
{
	name = "explosion";
	set_texture_from_name( "explosion" );
	set_duration( 1.f / 64 * 10 );
	set_frames_count( 10 );
	set_frame_size( { 128.0px, 128.0px } );

	auto sound = attach<Sound_Source>();
	sound->set_sound_buffer_from_name( "explosion" );
	sound->set_attenuation( 1 );
	sound->set_minimum_distance( 1000.0f );

	bond_disconnector( s_play.connect( [=]{ sound->play(); } ) );
}
