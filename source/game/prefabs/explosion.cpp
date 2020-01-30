/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "explosion.hpp"

#include "framework/scene/sound_source.hpp"

Explosion::Explosion()
{
	name = "explosion";
	set_texture_from_name( "explosion" );
	set_duration( 1.f / 48 * 10 );
	set_frames_count( 10 );
	set_frame_size( { 128.0px, 128.0px } );

	auto sound = attach<Sound_Source>();
	sound->name = "explosion_sound";
	sound->set_sound_buffer_from_name( "explosion" );
	// Setting it to 0 makes better results that to 1, because it fades out too quickly.
	sound->set_attenuation( 0 );

	bond_disconnector( s_play.connect( [=] { sound->play(); } ) );
}