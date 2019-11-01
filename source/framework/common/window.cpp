/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include <SFML/Window/Mouse.hpp>

#include "window.hpp"

namespace con::priv
{
void Window::initialize( u32 window_width, u32 window_height, u32 fps, std::string const& title )
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	window.create( { window_width, window_height }, title, sf::Style::Close, settings );
	window.setFramerateLimit( fps );

	fps_limit = fps;
}

auto Window::get_raw_window() -> sf::RenderWindow&
{
	return window;
}

auto Window::get_size() const -> Size2
{
	return window.getSize();
}

auto Window::get_bounds() const -> Rectangle_Shape
{
	return { {0,0}, window.getSize() };
}

auto Window::get_mouse_position() const -> Point
{
	return sf::Mouse::getPosition( window );
}

auto Window::get_fps_limit() const -> u32
{
	return fps_limit;
}

bool Window::is_focused() const
{
	return window.hasFocus();
}

Window& Window::get_instance()
{
	static Window* instance = new Window;

	return *instance;
}
}