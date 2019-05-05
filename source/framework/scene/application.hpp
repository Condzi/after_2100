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
	Root* get_root();

	void exit_game();

	static Application& get_instance();

private:
	Application() = default;

	sf::RenderWindow window;
	bool exit = false;
	Root root;
};
}