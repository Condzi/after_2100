/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "debug_console.hpp"

namespace con::priv
{
Debug_Console::Debug_Console()
{
	report_background.setFillColor( sf::Color{ 180, 180, 180, 150 } );
	input_background.setFillColor( sf::Color{ 140, 140, 140, 150 } );

	report_background.setSize( { 1280, ( LINES-1 ) * ( CHAR_SIZE + VERTICAL_SPACING ) } );
	input_background.setPosition( 0, report_background.getSize().y );
	input_background.setSize( { 1280, CHAR_SIZE + VERTICAL_SPACING } );

	font.loadFromFile( "resources/Oxanium-Regular.ttf" );

	for ( auto& text : visible_lines ) {
		text.setCharacterSize( CHAR_SIZE );
		text.setFont( font );
		text.setFillColor( sf::Color::White );
		text.setOutlineThickness( 1.0px );
		text.setOutlineColor( sf::Color::Black );
	}

	put_labels_on_correct_positions();
}

void Debug_Console::set_report_background_color( sf::Color const& color )
{
	report_background.setFillColor( color );
}

void Debug_Console::set_input_background_color( sf::Color const& color )
{
	input_background.setFillColor( color );
}

void Debug_Console::print( std::string const& new_message )
{
	history.emplace_back( new_message );

	if ( visible )
		update_lines();
}

void Debug_Console::update()
{
	if ( visible != previous_visible )
		if ( visible )
			update_lines();

	previous_visible = visible;
}

void Debug_Console::draw( Drawing_Set& set )
{
	if ( not visible )
		return;

	set.add_drawable( report_background, 100 );
	set.add_drawable( input_background, 100 );

	for ( auto& text : visible_lines )
		set.add_drawable( text, 100 );

	set.add_drawable( input_line, 100 );
}

Debug_Console& Debug_Console::get_instance()
{
	static Debug_Console* instance = new Debug_Console;

	return *instance;
}

void Debug_Console::put_labels_on_correct_positions()
{
	input_line.setPosition( { LEFT_MARGIN, input_background.getPosition().y + VERTICAL_SPACING } );

	for ( auto i = visible_lines.rbegin(); i != visible_lines.rend(); i++ ) {
		sf::Text& line = *i;
		constant idx = static_cast<size_t>( std::distance( visible_lines.rbegin(), i ) );

		line.setPosition( { LEFT_MARGIN, static_cast<r32>( VERTICAL_SPACING + idx * ( VERTICAL_SPACING + CHAR_SIZE ) ) } );
	}
}

void Debug_Console::update_lines()
{
	for ( size_t i = 0; i < visible_lines.size() and i < history.size(); i++ )
		visible_lines[i].setString( *( history.rbegin() + i ) );
}
}