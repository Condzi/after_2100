/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/
#pragma once

#include "shapes.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace con::priv
{
class Window final
{
public:
	void initialize( u32 window_width, u32 window_height, u32 fps, std::string const& title );

	[[nodiscard]] auto get_raw_window()           -> sf::RenderWindow&;
	[[nodiscard]] auto get_size() const           -> Size2;
	[[nodiscard]] auto get_bounds() const         -> Rectangle_Shape;
	[[nodiscard]] auto get_mouse_position() const -> Point;
	[[nodiscard]] auto get_fps_limit() const      -> u32;

	[[nodiscard]] bool is_focused() const;

	static Window& get_instance();

private:
	sf::RenderWindow window;
	u32 fps_limit{ 64 };
};
}

#define G_Window con::priv::Window::get_instance()