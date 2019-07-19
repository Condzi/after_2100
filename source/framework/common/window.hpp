/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "shapes.hpp"

namespace con::priv
{
class Window final
{
public:
	void initialize( u32 window_width, u32 window_height, u32 fps, std::string const& title );

	[[nodiscard]] auto get_raw_window()   -> sf::RenderWindow&;
	[[nodiscard]] auto get_size() const   -> Size2;
	[[nodiscard]] auto get_bounds() const -> Rectangle_Shape;

	static Window& get_instance();

private:
	sf::RenderWindow window;
};
}

#define G_Window con::priv::Window::get_instance()