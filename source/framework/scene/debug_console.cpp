/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "debug_console.hpp"

#include "framework/common/window.hpp"
#include "framework/localization/locale.hpp"
#include "framework/common/resources_storage.hpp"

namespace con::priv
{
Debug_Console::Debug_Console()
{
	report_background.setFillColor( sf::Color{ 140, 140, 140, 180 } );
	input_background.setFillColor( sf::Color{ 180, 180, 180, 180 } );
	input_background.setOutlineColor( sf::Color{ 30,130,30 } );

	report_background.setSize( { 1280, VERTICAL_SPACING + ( LINES-1 ) * ( CHAR_SIZE + VERTICAL_SPACING ) } );
	input_background.setPosition( 0, report_background.getSize().y );
	input_background.setSize( { 1280, CHAR_SIZE + VERTICAL_SPACING } );

	font.loadFromFile( "resources/Oxanium-Regular.ttf" );

	visible_lines = Rich_Text::instantiate();

	visible_lines->character_size = CHAR_SIZE;
	visible_lines->set_font_from_pointer( &font );
	visible_lines->fill_color = sf::Color::White;
	visible_lines->outline_color = sf::Color::Black;
	visible_lines->outline_thickness = 1.0px;
	visible_lines->layer = layer;

	input_text.setCharacterSize( CHAR_SIZE );
	input_text.setFont( font );
	input_text.setFillColor( sf::Color::White );
	input_text.setOutlineThickness( 1.0px );
	input_text.setOutlineColor( sf::Color::Black );

	input_sign = input_text;
	input_sign.setString( "<" );

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

void Debug_Console::print( sf::String const& new_message )
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
			( ascii < '0' or ascii > '9' ) and ascii is_not '_' )
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
		} else if ( event.key.code == sf::Keyboard::Up and
					scroll_offset < static_cast<s32>( history.size() ) - static_cast<s32>( LINES ) ) {
			scroll_offset++;
			update_lines();
		} else if ( event.key.code == sf::Keyboard::Down and scroll_offset > 0 ) {
			scroll_offset--;
			update_lines();
		}
	}

	input_text.setString( input_string );
	input_sign.setPosition( LEFT_MARGIN + input_text.getGlobalBounds().width, input_sign.getPosition().y );
}

void Debug_Console::update()
{
	if ( visible is_not previous_visible )
		if ( visible ) {
			input_focused = true;
			input_background.setOutlineThickness( -2 );
			update_lines();
		}

	previous_visible = visible;
}

void Debug_Console::draw( Drawing_Set& set )
{
	if ( not visible )
		return;

	set.add_drawable( report_background, layer );
	set.add_drawable( input_background, layer );

	visible_lines->draw_gui( set );

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
	input_text.setPosition( { LEFT_MARGIN, input_sign.getPosition().y } );

	visible_lines->set_global_position( { LEFT_MARGIN, 0 } );
}

void Debug_Console::update_lines()
{
	// it's a weird way to append lines, but it works like that so I don't touch it.
	constant append_new_line = [this]( sf::String line ) {
		auto current_string = visible_lines->string.get_string();
		line += current_string;

		visible_lines->string.set( line );
	};

	visible_lines->string.set( "" ); // clearing the string. @ToDo: add method to do that?

	for ( size_t i = 0; i < LINES and i < history.size(); i++ )
		append_new_line( *( history.rbegin() + i + scroll_offset ) );

	visible_lines->update_vertices();
}

void Debug_Console::do_command( std::string const& command )
{
	print( command + '\n' );

	constant first_space_idx = command.find_first_of( ' ' );
	constant lhs = lower_string( command.substr( 0, first_space_idx ) );
	constant rhs = lower_string( command.substr( first_space_idx + 1 ) );

	if ( lhs == "help" ) {
		for ( constant str : G_Debug_Flags.get_flags_names() )
			print( str + '\n' );

		print( "enable_all\n" );
		print( "disable_all\n" );
		print( "reload_locale\n" );
		print( "reload_resources\n" );

		return;
	}

	if ( first_space_idx is std::string::npos ) {
		if ( rhs == "enable_all" )
			G_Debug_Flags.enable_all();
		else if ( rhs == "disable_all" )
			G_Debug_Flags.disable_all();
		else if ( rhs == "reload_locale" )
			G_Locale.reload();
		else if ( rhs == "reload_resources" )
			G_Resources_Storage.reload();
		else
			engine_log_info( "{} = {}", lhs, G_Debug_Flags.get( lhs ) );
		return;
	}

	bool flag{ false };

	if ( rhs is "true" or rhs is "1" )
		flag = true;

	else if ( rhs is_not "false" and rhs is_not "0" ) {
		engine_log_warning( "Command error: expected 1/0, true/false, empty. Got: {}.", rhs );
		return;
	}

	G_Debug_Flags.get( lhs ) = flag;
}
}