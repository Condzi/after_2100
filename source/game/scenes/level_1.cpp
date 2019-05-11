/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "level_1.hpp"
#include "game/prefabs/player.hpp"

#include "framework/scene/path.hpp"
#include "framework/common/resources_storage.hpp"

Level_1::Level_1()
{
	name = "level_1";
	log_info( "{} instantiated.", name );

	attach( Player::instantiate() );

	///////////////////////////////
	Path& path = *attach( Path::instantiate() )->cast_to<Path>();
	path.draw_path = true;

	path.points.emplace_back( Point{ 1280.0px, 600.0px } );
	path.points.emplace_back( Point{ 800.0px,  600.0px } );
	path.points.emplace_back( Point{ 800.0px,  200.0px } );
	path.points.emplace_back( Point{ -100.0px, 200.0px } );

	///////////////////////////////
	Path_Follower& follower = *path.attach( Path_Follower::instantiate() )->cast_to<Path_Follower>();

	follower.set_path( path );

	///////////////////////////////
	Sprite& sprite = *follower.attach( Sprite::instantiate() )->cast_to<Sprite>();

	sprite.set_texture( G_Resources_Storage.get_texture( "enemy_1" ) );
	sprite.set_transformation_origin( sprite.get_global_bounds().size * 0.5 );
	sprite.rotate( -90.0deg );

	follower.start_following();
}
