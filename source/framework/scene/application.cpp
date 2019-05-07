/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/scene/application.hpp"
#include "framework/common/resources_storage.hpp"

namespace con
{
void Application::initialize( u32 window_width, u32 window_height, u32 fps, std::string const & title )
{
	window.create( { window_width, window_height }, title, sf::Style::Close );
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

		root.remove_queued_for_delete();
		root.update_children( fps_clock.restart().asSeconds() );

		window.clear();
		drawing_set.clear();
		root.draw_children( drawing_set );
		drawing_set.display( window );
		window.display();
	}
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
	static Application instance;

	return instance;
}
}