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

#include "framework/scene/label.hpp"
#include "framework/scene/music_source.hpp"

Level_1::Level_1()
{
	name = "level_1";
	log_info( "{} instantiated.", name );

	Label& l = *attach<Label>();

	l.string.set_locale_name( "#loc2" );
	l.set_fill_color( sf::Color::Cyan );
	l.set_absolute_position( Percent_Position{ 10.1, 1 } );
	attach<Player>();

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
	spawner.set_spawn_limit( 8 );

	Sprite* spr = attach<Sprite>();

	spr->set_texture_from_name( "space_background" );
	spr->layer = -1;

	auto music = attach<Music_Source>();

	music->set_music_from_name( "space_ambient_2" );
	music->set_loop( true );
	music->set_relative_to_audio_listener( true );
	music->play();
}
