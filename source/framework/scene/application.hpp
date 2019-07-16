/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

#include "root.hpp"
#include "camera.hpp"
#include "framework/common/shapes.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace con
{
class Application final
{
public:
	void initialize( u32 window_width, u32 window_height, u32 fps, std::string const& title );
	void run();

	// @ToDo: Probably somewhere else?
	[[nodiscard]] auto get_root()                -> Root&;
	// @ToDo: May be confusing:
	[[nodiscard]] auto get_window() const        -> sf::RenderWindow const&;
	[[nodiscard]] auto get_window_size() const   -> Size2;
	[[nodiscard]] auto get_window_bounds() const -> Rectangle_Shape;

	void exit_game();

	static Application& get_instance();

private:
	sf::RenderWindow window;
	Drawing_Set      game_drawing_set;
	Drawing_Set      gui_drawing_set;
	Camera*          game_camera;
	Camera*          gui_camera;

	bool             exit{ false };
	Root             root;

	Application() = default;

	void handle_debug_keys( sf::Event const& event );
	void render();
};
}

#define G_App con::Application::get_instance()
#define G_Window G_App.get_window()
#define G_Root G_App.get_root()