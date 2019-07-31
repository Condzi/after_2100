/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include "framework/common/drawing_set.hpp"
#include "framework/common/debug_flags.hpp"

namespace con::priv
{
class Debug_Console final
{
public:
	s32 layer = 100;

	void set_report_background_color( sf::Color const& color );
	void set_input_background_color( sf::Color const& color );
	void set_input_outline_selection_color( sf::Color const& color );

	void print( std::string const& new_message );

	void input( sf::Event const& event );
	void update();
	void draw( Drawing_Set& set );

	static Debug_Console& get_instance();

private:
	Debug_Console();

	compile_constant LINES = 12; // 15 + 1 for input
	compile_constant CHAR_SIZE = 16;
	compile_constant VERTICAL_SPACING = 4;
	compile_constant HEIGHT = LINES * ( CHAR_SIZE + VERTICAL_SPACING );
	compile_constant LEFT_MARGIN = 1.0px;

	// Has own copy of the default font to be independent.
	sf::Font font;

	bool const& visible = G_Debug_Flags.get( "display_debug_console" );
	bool previous_visible = false; // when was invisible in previous frame and is visinle now then update in current

	sf::RectangleShape report_background;
	sf::RectangleShape input_background;
	bool input_focused{ false };

	std::vector<std::string> history; // FILO ~ first message is the oldest one.
	std::array<sf::Text, LINES - 1> visible_lines; // The first message is the bottom one.
	sf::Text input_sign;
	sf::Text input_text;

	void put_labels_on_correct_positions(); // when initializing for the first time 
	void update_lines(); // after printing or scrolling
	void do_command( std::string const& command );
};
}

#define G_Debug_Console con::priv::Debug_Console::get_instance()