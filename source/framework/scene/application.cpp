/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/scene/application.hpp"
#include "framework/common/resources_storage.hpp"
#include "framework/common/stl_extensions.hpp"

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
		}

		for ( auto signal : signals )
			signal->remove_invalid_subscribers();

		root.remove_queued_for_delete();
		root.update_children( fps_clock.restart().asSeconds() );

		window.clear();
		drawing_set.clear();
		root.draw_children( drawing_set );
		drawing_set.display( window );
		window.display();
	}

	engine_log_info( "Exiting game loop..." );
}

void Application::_add_signal( priv::Signal_Base* signal_to_add )
{
	constant[found, idx] = find( signals, signal_to_add );

	report_error_if( found )
	{
		engine_log_error( "Trying to add already added signal. Duplicate found at index {}.", idx );
		return;
	}

	signals.emplace_back( signal_to_add );
}

void Application::_remove_signal( priv::Signal_Base* signal_to_remove )
{
	constant[found, idx] = find( signals, signal_to_remove );

	report_error_if( not found )
	{
		engine_log_error( "Trying to remove non existing signal." );
		return;
	}

	signals.erase( signals.begin() + idx );
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
	// When signal wanted to detach from G__App and App was destroyed,
	// a new instance had been made that caused bugs.
	static Application* instance = new Application;

	return *instance;
}
}