/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "level_1.hpp"
#include "game/prefabs/player.hpp"
#include "game/prefabs/enemy_base.hpp"

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
	Enemy_Base& enemy = *attach( Enemy_Base::instantiate() )->cast_to<Enemy_Base>();

	enemy.set_path( path );
	enemy.start_following();
}
