/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/scene/application.hpp"
#include "framework/common/resources_storage.hpp"
#include "framework/common/stl_extensions.hpp"
#include "framework/scene/area_overleaping_checker.hpp"

namespace con
{
void Application::initialize( u32 window_width, u32 window_height, u32 fps, std::string const& title )
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	window.create( { window_width, window_height }, title, sf::Style::Close, settings );
	window.setFramerateLimit( fps );

	G_Resources_Storage.reload();
}

void Application::run()
{
	sf::Clock fps_clock;
	sf::Event event;

	while ( not exit ) {
		while ( window.pollEvent( event ) ) {
			root.handle_input_children( event );
			handle_debug_keys( event );
		}

		root.remove_queued_for_delete();
		root.update_children( fps_clock.restart().asSeconds() );

		G_Area_Overleaping_Checker.update();

		window.clear();
		drawing_set.clear();
		root.draw_children( drawing_set );
		drawing_set.display( window );
		window.display();
	}

	engine_log_info( "Exiting game loop..." );
}

auto Application::get_root() -> Root &
{
	return root;
}

auto Application::get_window() const -> sf::RenderWindow const&
{
	return window;
}

auto Application::get_window_size() const -> Size2
{
	return static_cast<Size2>( window.getSize() );
}

void Application::exit_game()
{
	exit = true;
}

Application& Application::get_instance()
{
	// @Hack: Prevent from destroying on exit.
	// When signal wanted to detach from G_App and App was destroyed,
	// a new instance had been made that caused bugs.
	static Application* instance = new Application;

	return *instance;
}

void Application::handle_debug_keys( sf::Event const& event )
{
	if ( event.type != sf::Event::EventType::KeyPressed )
		return;

	switch ( event.key.code ) {
	case sf::Keyboard::Escape:
	{
		exit_game();
		break;
	}
	case sf::Keyboard::F1:
	{
		G_Debug_Flags.enable_all();
		engine_log_info( "Debug: enable all." );
		break;
	}
	case sf::Keyboard::F2:
	{
		G_Debug_Flags.disable_all();
		engine_log_info( "Debug: disable all." );
		break;
	}
	case sf::Keyboard::F3:
	{
		G_Debug_Flags.draw_areas = !G_Debug_Flags.draw_areas;

		engine_log_info( "Debug: toggle draw_areas." );
		break;
	}
	case sf::Keyboard::F4:
	{
		G_Debug_Flags.draw_paths = !G_Debug_Flags.draw_paths;
		engine_log_info( "Debug: toggle draw_paths." );
		break;
	}
	case sf::Keyboard::F5:
	{
		G_Debug_Flags.draw_missile_shooters = !G_Debug_Flags.draw_missile_shooters;
		engine_log_info( "Debug: toggle draw_missile_shooters." );
		break;
	}
	}
}
}