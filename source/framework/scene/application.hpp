/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

#include "node.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace con
{
class Application final
{
public:
	Application( u32 window_width, u32 window_height, u32 fps, std::string const& title );

	void run();

	// @ToDo: Probably somewhere else?
	Node* get_root();

	void exit_game();

private:
	sf::RenderWindow window;
	bool exit = false;
	// @ToDo: Maybe special class, inherited from Node, that handles saving, exiting from game?
	// And scene changing.
	Node root;
};
}