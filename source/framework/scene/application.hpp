/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

#include "camera.hpp"
#include "root.hpp"

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
	[[nodiscard]] auto get_root()  -> Root&;
	[[nodiscard]] auto is_paused() const -> bool;

	void exit_game();
	// Just toggles flag. It's your job to handle it.
	void toggle_pause();

	static Application& get_instance();

private:
	Drawing_Set game_drawing_set;
	Drawing_Set gui_drawing_set;
	Camera* game_camera;
	Camera* gui_camera;

	bool exit{ false };
	bool paused{ false };
	Root root;

	Application() = default;

	void render();
};
}

#define G_App con::Application::get_instance()
#define G_Root G_App.get_root()