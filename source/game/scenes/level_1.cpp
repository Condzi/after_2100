/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/resources_storage.hpp"
#include "framework/scene/application.hpp"

#include "game/flags.hpp"

#include "level_1.hpp"

Level_1::Level_1()
{
	name = "level_1";

	player = attach<Player>();

	enemy_spawner = attach<Enemy_Spawner>();
	enemy_path_1  = attach<Path>();
	enemy_path_2  = attach<Path>();
	enemy_path_3  = attach<Path>();

	camera_path = attach<Path>();

	parallax_background = attach<Parallax_Background>();
	music_source        = attach<Music_Source>();

	constant win_size = G_Window.get_size();

	///////////////////////////////
	enemy_path_1->points.emplace_back( win_size * Vec2{ 1.1, 0.5 } );

	for ( r32 i = 10; i > 0; i -= 0.05 )
		enemy_path_1->points.emplace_back( win_size.width * i / 10, win_size.height * sin( i ) * 0.2f + win_size.height * 0.5f );

	enemy_path_1->points.emplace_back( win_size * Vec2{ -0.1, 0.5 } );

	///////////////////////////////
	enemy_path_2->points.emplace_back( win_size * Vec2{ 1.1, 1.1 } );
	enemy_path_2->points.emplace_back( win_size * Vec2{ -0.1, -0.1 } );
	///////////////////////////////
	enemy_path_3->points.emplace_back( win_size * Vec2{ 1.1, -0.1 } );
	enemy_path_3->points.emplace_back( win_size * Vec2{ -0.1, 1.1 } );

	///////////////////////////////
	enemy_spawner->set_path( *enemy_path_1 );
	enemy_spawner->set_enemy_type<Enemy_Base>();
	enemy_spawner->spawn_interval = 1.9sec;
	enemy_spawner->set_spawn_limit( 10 );
	enemy_spawner->start();

	// switch paths when one ends.
	bond_disconnector( enemy_spawner->s_finished.connect( [spawner = enemy_spawner, p_2 = enemy_path_2, p_3 = enemy_path_3] {
		if ( spawner->spawn_interval is 1.9sec ) {
			spawner->set_path( *p_2 );
			spawner->spawn_interval = 1.5sec;
		} else if ( spawner->spawn_interval is 1.5sec ) {
			spawner->set_path( *p_3 );
			spawner->spawn_interval = 1.3sec;
		} else
			return;

		spawner->reset();
		spawner->start();
					   } ) );

	///////////////////////////////
	parallax_background->set_texture_from_name( "space_background" );
	parallax_background->layer = 0;
	parallax_background->set_absolute_position( Percent_Position{ -10, -10 } );
	parallax_background->speed = 100;

	///////////////////////////////
	music_source->name = "space_ambient";
	music_source->set_music_from_name( "space_ambient_1" );
	music_source->set_relative_to_audio_listener( true );
	music_source->play();
	// (when music finishes, randomize next)
	bond_disconnector( music_source->s_music_stop.connect( [this] {
		constant num = std::to_string( random_int( 1, 3 ) );
		auto music = get_node( "space_ambient" )->cast_to<Music_Source>();
		music->set_music_from_name( "space_ambient_" + num );
		music->stop(); //For some reason we have to stop it before playing again, otherwise if the same music is randomized then it won't play.
		music->play();

		log_info( "Next track: space_ambient_{}", num );
					   } ) );

	///////////////////////////////
	camera_path->points.emplace_back( Point{ -5, 50 } *0.01 * win_size );
	camera_path->points.emplace_back( Point{ 50,50 } *0.01 * win_size );
	camera_path->visual_representation_color = sf::Color( 125, 100, 75 );

	auto& camera = *G_Root.get_node( "game_camera" )->cast_to<Camera>();
	camera.set_rotation( 90.0deg );
	camera.s_update.connect( [cam = &camera]( r32 dt ) {
		if ( G_Flags[Flags::Pause] )
			return;

		constant rotation_delta = -16.0deg * dt;
		if ( cam->get_rotation() + rotation_delta < 0 )
			cam->set_rotation( 0.0deg );
		else
			cam->rotate( rotation_delta );
							 } );
	camera.max_velocity = 130;
	camera.minimum_distance = 1.0px;
	camera.set_path( *camera_path );
	camera.start_following();
	///////////////////////////////

	log_info( "{} instantiated.", name );
}

void Level_1::update( r32 dt )
{
	// @ToDo: Probably inefficent?
	for ( auto* child : get_children() )
		child->set_pause( G_Flags[Flags::Pause] );
}