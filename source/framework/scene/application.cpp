/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "application.hpp"
#include "area_overleaping_checker.hpp"

#include "framework/common/audio_listener.hpp"
#include "framework/common/debug_flags.hpp"
#include "framework/common/resources_storage.hpp"
#include "framework/common/stl_extensions.hpp"
#include "framework/common/window.hpp"

#include "framework/localization/locale.hpp"
#include "framework/scene/debug_console.hpp"

namespace con
{
void Application::initialize()
{
	game_camera = root.attach<Camera>();
	gui_camera = root.attach<Camera>();

	game_camera->name = "game_camera";
	gui_camera->name = "gui_camera";

	constant view = G_Window.get_raw_window().getView();
	game_camera->set_view( view );
	gui_camera->set_view( view );
}

void Application::run()
{
	sf::Clock fps_clock;
	sf::Event event;
	r32 frame_time{ 0 };
	bool is_window_focused = true;

	while ( not exit ) {
		while ( G_Window.get_raw_window().pollEvent( event ) ) {
			if ( event.type is sf::Event::LostFocus ) {
				G_Audio_Listener.mute();
				is_window_focused = false;
			} else if ( event.type is sf::Event::GainedFocus ) {
				is_window_focused = true;
				G_Audio_Listener.unmute();
			} else if ( event.type is sf::Event::KeyPressed and
						event.key.code is sf::Keyboard::F1 ) {
				G_Debug_Flags.toggle( "display_debug_console" );
			}

			// Waiting for GainedFocus event.
			if ( is_window_focused is false )
				continue;

			root.handle_input_children( event );
			G_Debug_Console.input( event );
		}

		root.remove_queued_for_delete();
		root.update_children( frame_time );
		G_Debug_Console.update();

		G_Area_Overleaping_Checker.update();
		render();

		frame_time = fps_clock.restart().asSeconds();
	}

	engine_log_info( "Exiting game loop..." );
}

auto Application::get_root() -> Root&
{
	return root;
}

auto Application::is_paused() const -> bool
{
	return paused;
}

void Application::exit_game()
{
	exit = true;
}

void Application::toggle_pause()
{
	paused = !paused;
}

Application& Application::get_instance()
{
	// @Hack: Prevent from destroying on exit.
	// When signal wanted to detach from G_App and App was destroyed,
	// a new instance had been made that caused bugs.
	static Application* instance = new Application;

	return *instance;
}

void Application::render()
{
	game_drawing_set.clear();
	gui_drawing_set.clear();

	root.draw_children( game_drawing_set );
	root.draw_gui_children( gui_drawing_set );

	G_Debug_Console.draw( gui_drawing_set );

	auto& window = G_Window.get_raw_window();

	// @ToDo: Clear to black.
	window.clear( sf::Color{ 14,19,22 } );

	game_drawing_set.display( window );
	window.setView( gui_camera->get_view() );
	gui_drawing_set.display( window );
	window.setView( game_camera->get_view() );

	window.display();
}
}