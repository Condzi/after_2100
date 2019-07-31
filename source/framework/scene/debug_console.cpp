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
	name = "debug_console";
	report_background.setFillColor( sf::Color{ 180, 180, 180, 150 } );
	input_background.setFillColor( sf::Color{ 140, 140, 140, 150 } );

	report_background.setSize( { G_Window.get_size().width, ( LINES-1 ) * ( CHAR_SIZE + VERTICAL_SPACING ) } );
	input_background.setPosition( 0, report_background.getSize().y );
	input_background.setSize( { G_Window.get_size().width, CHAR_SIZE + VERTICAL_SPACING } );

	input_line = attach<Label>();
	bond_disconnector( input_line->s_update.connect( [this]( r32 dt ) { input_line->visible = this->visible; } ) );

	for ( auto& label : visible_lines ) {
		label = attach<Label>();
		label->set_character_size( CHAR_SIZE );
		label->set_font( FONT_NAME );
		label->layer = layer + 1;
		label->set_fill_color( sf::Color::White );
		label->set_outline_thickness( 1.0px );
		label->set_outline_color( sf::Color::Black );

		bond_disconnector( label->s_update.connect( [label, this]( r32 dt ) { label->visible = this->visible; } ) );
	}

	put_labels_on_correct_positions();

	print( "first message" );
	print( "second message" );
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

void Debug_Console::update( r32 dt )
{
	if ( visible != previous_visible )
		if ( visible )
			update_lines();

	previous_visible = visible;
}

void Debug_Console::draw_gui( Drawing_Set& set )
{
	if ( not visible )
		return;

	set.add_drawable( report_background, layer );
	set.add_drawable( input_background, layer );
}

void Debug_Console::put_labels_on_correct_positions()
{
	input_line->set_global_position( { LEFT_MARGIN, input_background.getPosition().y + VERTICAL_SPACING } );

	for ( auto i = visible_lines.rbegin(); i != visible_lines.rend(); i++ ) {
		Label& line = *( *i );
		constant idx = static_cast<size_t>( std::distance( visible_lines.rbegin(), i ) );

		line.set_global_position( { LEFT_MARGIN, static_cast<r32>( VERTICAL_SPACING + idx * ( VERTICAL_SPACING + CHAR_SIZE ) ) } );
	}
}

void Debug_Console::update_lines()
{
	for ( size_t i = 0; i < visible_lines.size() and i < history.size(); i++ )
		visible_lines[i]->string.set_ascii( *( history.rbegin() + i ) );
}
}