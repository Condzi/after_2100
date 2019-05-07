/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

#include "root.hpp"

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
	auto get_root()                -> Root&;
	auto get_window() const        -> sf::RenderWindow const&;

	void exit_game();

	static Application& get_instance();

private:
	Application() = default;

	sf::RenderWindow window;
	Drawing_Set drawing_set;
	bool exit = false;
	Root root;
};
}

#define G_App con::Application::get_instance()
#define G_Window G_App.get_window()
#define G_Root G_App.get_root()