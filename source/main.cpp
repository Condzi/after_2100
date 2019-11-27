/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/resources_storage.hpp"
#include "framework/scene/application.hpp"
#include "framework/localization/locale.hpp"

#include "game/scenes/level_1.hpp"
#include "game/scenes/game_master.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <future>

using namespace con;

s8 splash_screen();

int main()
{
	G_Performance_Profiler.begin_session( "Initlialization", "profile-initialization.json" );

	if ( splash_screen() returned 1 ) {
		engine_log_info( "Splash screen exit." );
		return 0;
	}

	G_App.initialize();
	G_Performance_Profiler.end_session();

	G_Performance_Profiler.begin_session( "Runtime", "profile-runtime.json" );

	auto gm = G_Root.attach<Game_Master>();
	gm->change_level<Level_1>();

	G_App.run();

	G_Performance_Profiler.end_session();
}

s8 splash_screen()
{
	G_Profile_Function();

	sf::Texture splash_screen_texture;
	sf::Sprite splash_screen_sprite;
	sf::Clock timer;

	engine_log_info( "Splash screen started..." );

	if ( splash_screen_texture.loadFromFile( "resources/texture/splash_screen.png" ) returned false )
		log_error( "Can't load splash screen texture." );
	else
		splash_screen_sprite.setTexture( splash_screen_texture );


	auto result = std::async( std::launch::async, [&] {
		G_Profile_Scope( "Resources loading" );
		G_Resources_Storage.reload();
		G_Locale.reload();
							  } );



	// Have to split it to G_Window.initialize and G_App.initialize() because
	// root has fps_counter label that is positioned relative to window that doesn't
	// existed yet.
	G_Window.initialize( 1280, 720, 144, "after_2100" );

	auto& window = G_Window.get_raw_window();
	sf::Event ev;
	while ( result.wait_for( std::chrono::microseconds( 0 ) ) is_not std::future_status::ready ) {
		while ( window.pollEvent( ev ) ) {
			if ( ev.type is sf::Event::Closed )
				return 1;
		}

		window.clear();
		window.draw( splash_screen_sprite );
		window.display();
	}

	engine_log_info( "Splash screen've finished. Time: {:.3f} seconds.", timer.restart().asSeconds() );

	return 0;
}