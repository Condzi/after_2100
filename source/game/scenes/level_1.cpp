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

#include "framework/scene/rich_text.hpp"

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
	spr->set_absolute_position( Percent_Position{ -5, -5 } );

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

	auto rt = attach<Rich_Text>();

	rt->set_font_from_name( "default" );
	rt->string.set_locale_name( "#loc2" );
	rt->string.update_locale_parameters( L"69 420 69" );
	rt->character_size = 40;
	rt->outline_thickness = 3.0px;
	rt->set_absolute_position( Percent_Position{ 10, 20 } );
	rt->outline_color = sf::Color::Cyan;
	rt->update_vertices();

	auto rt_area = rt->attach<Area>();
	rt_area->collision_layer = -1;
	rt_area->set_rectangle_shape( rt->get_global_bounds() );
	Point p = rt->find_character_position( 4 );

	auto pos_area = attach<Area>();

	pos_area->set_circle_shape( {p, 2.0px} );

	log_info( "Character position: {:.1f}, {:.1f}", p.x, p.y );
	log_info( "Real string: {}", rt->get_string_without_formating_characters().toAnsiString() );

	attach<Player>();
}
