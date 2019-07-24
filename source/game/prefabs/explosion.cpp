/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "pch.hpp"
#include "explosion.hpp"

Explosion::Explosion()
{
	name = "explosion";
	set_texture_from_pointer( G_Resources_Storage.get_texture( "explosion" ) );
	set_duration( 0.5sec );
	set_frames_count( 10 );
	set_frame_size( { 128.0px, 128.0px } );
}
