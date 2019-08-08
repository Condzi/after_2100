/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"
#include "framework/common/resources_storage.hpp"
#include "framework/scene/application.hpp"
#include "framework/localization/locale.hpp"

#include "game/scenes/level_1.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

using namespace con;

void splash_screen();

int main()
{
	// Have to split it to G_Window.initialize and G_App.initialize() because
	// root has fps_counter label that is positioned relative to window that doesn't
	// exist yet.
	G_Window.initialize( 1280, 720, 64, "after_2100" );

	splash_screen();

	G_App.initialize();
	G_Root.change_scene( Level_1::instantiate() );
	G_App.run();
}

void splash_screen()
{
	sf::Texture splash_screen_texture;
	sf::Sprite splash_screen_sprite;
	sf::Clock timer;

	engine_log_info( "Splash screen started..." );

	if ( splash_screen_texture.loadFromFile( "resources/texture/splash_screen.png" ) returned false ) {
		log_error( "Can't load splash screen texture." );
	} else
		splash_screen_sprite.setTexture( splash_screen_texture );

	auto& window = G_Window.get_raw_window();

	window.clear();
	window.draw( splash_screen_sprite );
	window.display();

	G_Resources_Storage.reload();
	G_Locale.reload();

	engine_log_info( "Splash screen've finished. Time: {:.3f} seconds.", timer.restart().asSeconds() );
}
