/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "debug_console.hpp"

#include "framework/common/window.hpp"

namespace con::priv
{
Debug_Console::Debug_Console()
{
	report_background.setFillColor( sf::Color{ 180, 180, 180, 150 } );
	input_background.setFillColor( sf::Color{ 140, 140, 140, 150 } );
	input_background.setOutlineColor( sf::Color{ 30,130,30 } );

	report_background.setSize( { 1280, VERTICAL_SPACING + ( LINES-1 ) * ( CHAR_SIZE + VERTICAL_SPACING ) } );
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

	input_text.setCharacterSize( CHAR_SIZE );
	input_text.setFont( font );
	input_text.setFillColor( sf::Color::White );
	input_text.setOutlineThickness( 1.0px );
	input_text.setOutlineColor( sf::Color::Black );

	input_sign = input_text;
	input_sign.setString( "] " );

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

void Debug_Console::set_input_outline_selection_color( sf::Color const& color )
{
	input_background.setOutlineColor( color );
}

void Debug_Console::print( std::string const& new_message )
{
	history.emplace_back( new_message );

	if ( visible )
		update_lines();
}

void Debug_Console::input( sf::Event const& event )
{
	if ( not visible )
		return;

	if ( event.type is sf::Event::EventType::MouseButtonReleased and
		 event.mouseButton.button is sf::Mouse::Button::Left ) {

		constant mouse_position = G_Window.get_mouse_position();
		constant input_bounds = Rectangle_Shape( input_background.getGlobalBounds() );

		input_focused = rect_vs_point( input_bounds, mouse_position );

		input_background.setOutlineThickness( input_focused ? -2 : 0 );
	}

	if ( not input_focused )
		return;

	std::string input_string = input_text.getString();

	if ( event.type is sf::Event::EventType::TextEntered ) {
		char ascii = sf::String{ event.text.unicode }.toAnsiString().front();

		if ( ( ascii < 'A' or ascii > 'Z' ) and ( ascii < 'a' or ascii > 'z' ) and
			( ascii < '0' or ascii > '9' ) )
			return;

		input_string += ascii;
	}

	if ( event.type is sf::Event::EventType::KeyReleased ) {

		if ( event.key.code == sf::Keyboard::Key::Space )
			input_string += ' ';
		else if ( event.key.code == sf::Keyboard::Key::BackSpace and
				  not input_string.empty() )
			input_string.pop_back();
		else if ( event.key.code == sf::Keyboard::Key::Return ) {
			do_command( input_string );
			input_string.clear();
		}
	}

	input_text.setString( input_string );
}

void Debug_Console::update()
{
	if ( visible is_not previous_visible )
		if ( visible )
			update_lines();

	previous_visible = visible;
}

void Debug_Console::draw( Drawing_Set& set )
{
	if ( not visible )
		return;

	set.add_drawable( report_background, layer );
	set.add_drawable( input_background, layer );

	for ( auto& text : visible_lines )
		set.add_drawable( text, layer );

	set.add_drawable( input_sign, layer );
	set.add_drawable( input_text, layer );
}

Debug_Console& Debug_Console::get_instance()
{
	static Debug_Console* instance = new Debug_Console;

	return *instance;
}

void Debug_Console::put_labels_on_correct_positions()
{

	input_sign.setPosition( { LEFT_MARGIN, input_background.getPosition().y } );
	input_text.setPosition( input_sign.getPosition().x + input_sign.getGlobalBounds().width, input_sign.getPosition().y );


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

void Debug_Console::do_command( std::string const& command )
{
	//@ToDo
}
}