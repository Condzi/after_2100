/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "pch.hpp"

#include "root.hpp"
#include "framework/common/signal.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace con
{
class Application final
{
public:
	void initialize( u32 window_width, u32 window_height, u32 fps, std::string const& title );
	void run();

	// Internal:
	void _register_signal( priv::Signal_Base* signal_to_add );
	void _unregister_signal( priv::Signal_Base* signal_to_remove );

	// @ToDo: Probably somewhere else?
	auto get_root()              -> Root &;
	// @ToDo: May be confusing:
	auto get_window() const      -> sf::RenderWindow const&;
	auto get_window_size() const -> Size2;

	void exit_game();

	static Application& get_instance();

private:
	sf::RenderWindow window;
	Drawing_Set      drawing_set;
	bool             exit{ false };
	Root             root;

	std::vector<priv::Signal_Base*> signals;

	Application() = default;

	void handle_debug_keys( sf::Event const& event );
};
}

#define G_App con::Application::get_instance()
#define G_Window G_App.get_window()
#define G_Root G_App.get_root()