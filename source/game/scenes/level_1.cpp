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

#include "framework/scene/music_source.hpp"


Level_1::Level_1()
{
	name = "level_1";
	log_info( "{} instantiated.", name );

	///////////////////////////////
	Path& path = *attach<Path>();

	constant win_size = G_Window.get_size();

	path.points.emplace_back( win_size * Vec2{ 1.1, 0.5 } );

	for ( r32 i = 10; i > 0; i -= 0.05 )
		path.points.emplace_back( win_size.width * i / 10, win_size.height * sin( i ) * 0.2f + win_size.height * 0.5f );

	path.points.emplace_back( win_size * Vec2{ -0.1, 0.5 } );

	///////////////////////////////
	Enemy_Spawner& spawner = *attach<Enemy_Spawner>();
	spawner.set_path( path );
	spawner.set_enemy_type<Enemy_Base>();
	spawner.start();
	spawner.spawn_interval = 1.9sec;
	spawner.set_spawn_limit( 200 );

	Sprite* spr = attach<Sprite>();

	spr->set_texture_from_name( "space_background" );
	spr->layer = 0;
	spr->set_absolute_position( Percent_Position{ -10, -10 } );

	auto music = attach<Music_Source>();

	music->set_music_from_name( "space_ambient_1" );
	music->set_relative_to_audio_listener( true );
	music->play();
	music->name = "space_ambient";
	// (when music finishes, randomize next)
	bond_disconnector( music->s_music_stop.connect( [this] {
		constant num = std::to_string( random_int( 1, 3 ) );
		auto music = get_node( "space_ambient" )->cast_to<Music_Source>();
		music->set_music_from_name( "space_ambient_" + num );
		music->stop(); //@Bug: for some reason we have to stop it before playing again, otherwise if the same music is randomized then it won't play.
		music->play();

		log_info( "Next track: space_ambient_{}", num );
					   } ) );

	attach<Player>();

	auto cam_path = attach<Path>();

	cam_path->points.emplace_back( Point{ 0, 50 } *0.01 * win_size );
	cam_path->points.emplace_back( Point{ 15,50 } *0.01 * win_size );
	cam_path->points.emplace_back( Point{ 20,50 } *0.01 * win_size );
	cam_path->points.emplace_back( Point{ 30,50 } *0.01 * win_size );
	cam_path->points.emplace_back( Point{ 50,50 } *0.01 * win_size );
	cam_path->visual_representation_color = sf::Color( 125, 100, 75 );

	auto& camera = *G_Root.get_node( "game_camera" )->cast_to<Camera>();
	camera.set_rotation( 90.0deg );
	camera.set_zoom( 0.1 );
	camera.s_update.connect( [cam = &camera]( r32 dt ) {
		if ( G_App.is_paused() )
			return;

		constant rotation_delta = -18.0deg * dt;
		constant zoom_delta = 0.18 * dt;

		if ( cam->get_rotation() + rotation_delta < 0 )
			cam->set_rotation( 0.0deg );
		else
			cam->rotate( rotation_delta );

		if ( cam->get_zoom() + zoom_delta > 1.0 )
			cam->set_zoom( 1.0 );
		else
			cam->zoom( zoom_delta );
							 } );
	camera.max_velocity = 130;
	camera.set_path( *cam_path );
	camera.start_following();
}

void Level_1::update( r32 dt )
{
	// We don't unpause here - we do it in Pause_Screen class by
	// calling G_Root.set_pause(false).
	if ( G_App.is_paused() )
		set_pause( true );
}
