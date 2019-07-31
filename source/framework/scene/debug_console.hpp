/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "label.hpp"
#include "framework/common/debug_flags.hpp"
#include "framework/common/resources_storage.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

namespace con::priv
{
class Debug_Console final :
	public Node
{
	CLASS_DEF( Debug_Console );

public:
	s32 layer{ 100 }; // on top of everything

	Debug_Console();

	void set_report_background_color( sf::Color const& color );
	void set_input_background_color( sf::Color const& color );

	void print( std::string const& new_message );

	void update( r32 dt ) override;
	void draw_gui( Drawing_Set& set ) override;

private:
	compile_constant LINES = 16; // 15 + 1 for input
	compile_constant CHAR_SIZE = 8;
	compile_constant VERTICAL_SPACING = 2;
	compile_constant HEIGHT = LINES * ( CHAR_SIZE + VERTICAL_SPACING );
	compile_constant LEFT_MARGIN = 5.0px;

	compile_constant FONT_NAME = "default";
	sf::Font const& font = *G_Resources_Storage.get_font( FONT_NAME ); //@TODO: Unused?

	bool const& visible = G_Debug_Flags.get( "display_debug_console" );
	bool previous_visible = false; // when was invisible in previous frame and is visinle now then update in current

	sf::RectangleShape report_background;
	sf::RectangleShape input_background;

	std::vector<std::string> history; // FILO ~ first message is the oldest one.
	std::array<Label*, LINES - 1> visible_lines; // The first message is the bottom one.
	Label* input_line;

	void put_labels_on_correct_positions(); // when initializing for the first time 
	void update_lines(); // after printing or scrolling
};
}

// G_Debug_Console -> get from root