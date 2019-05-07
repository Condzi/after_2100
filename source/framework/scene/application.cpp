/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/scene/application.hpp"

namespace con
{
void Application::initialize( u32 window_width, u32 window_height, u32 fps, std::string const & title )
{
	window.create( { window_width, window_height }, title, sf::Style::Close );
	window.setFramerateLimit( fps );
}

void Application::run()
{
	sf::Clock fps_clock;
	sf::Event event;

	while ( not exit ) {
		while ( window.pollEvent( event ) ) {
			root.handle_input_children( event );
		}

		root.update_children( fps_clock.restart().asSeconds() );

		window.clear();
		drawing_queue.clear();
		root.draw_children( drawing_queue );
		drawing_queue.display( window );
		window.display();
	}
}

Root* Application::get_root()
{
	return &root;
}

void Application::exit_game()
{
	exit = true;
}
Application& Application::get_instance()
{
	static Application instance;

	return instance;
}
}