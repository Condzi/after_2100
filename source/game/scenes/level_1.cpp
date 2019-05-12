/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "level_1.hpp"
#include "game/prefabs/player.hpp"
#include "game/prefabs/enemy_spawner.hpp"

#include "framework/scene/path.hpp"
#include "framework/common/resources_storage.hpp"
#include "framework/scene/application.hpp"

Level_1::Level_1()
{
	name = "level_1";
	log_info( "{} instantiated.", name );

	attach( Player::instantiate() );

	///////////////////////////////
	Path& path = *attach( Path::instantiate() )->cast_to<Path>();
	path.draw_path = true;

	constant w = G_App.get_window_size().width;
	constant h = G_App.get_window_size().height;

	path.points.emplace_back( Point{ w * 1.1,   h * 0.5 } );

	for ( s32 i = 0; i < 5; i++ ) {
		path.points.emplace_back( Point{ w * ( 0.95 - 0.20*i ),  h * 0.5 } );
		path.points.emplace_back( Point{ w * ( 0.95 - 0.20*i ),  h * 0.1 } );
		path.points.emplace_back( Point{ w * ( 0.85 - 0.20*i ),  h * 0.1 } );
		path.points.emplace_back( Point{ w * ( 0.85 - 0.20*i ),  h * 0.9 } );
		path.points.emplace_back( Point{ w * ( 0.92 - 0.20*i ),  h * 0.7 } );
		path.points.emplace_back( Point{ w * ( 0.80 - 0.20*i ),  h * 0.5 } );
	}

	path.points.emplace_back( Point{ w * -0.1,   h * 0.5 } );

	///////////////////////////////
	Enemy_Spawner& spawner = *attach( Enemy_Spawner::instantiate() )->cast_to<Enemy_Spawner>();
	spawner.set_path( path );
	spawner.set_enemy_type<Enemy_Base>();
	spawner.start();
	spawner.spawn_interval = 1.5sec;
	spawner.set_spawn_limit( 8 );
}
