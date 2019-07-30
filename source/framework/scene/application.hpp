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
	void initialize();
	void run();

	// @ToDo: Probably somewhere else?
	[[nodiscard]] auto get_root() -> Root&;

	void exit_game();

	static Application& get_instance();

private:
	Drawing_Set      game_drawing_set;
	Camera*          game_camera;
	Drawing_Set      gui_drawing_set;
	Camera*          gui_camera;

	bool             exit{ false };
	Root             root;

	Application() = default;

	void handle_debug_keys( sf::Event const& event );
	void render();
};
}

#define G_App con::Application::get_instance()
#define G_Root G_App.get_root()