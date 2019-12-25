/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "input_field.hpp"

namespace con
{
Input_Field::Input_Field()
{
	text_visual = attach<Rich_Text>();

	text_visual->name = "text_visual";
	text_visual->outline_color = sf::Color::White;
	text_visual->outline_thickness = 1.0px;

	text_color_default = sf::Color{ 190,190,190 };
	text_color_input   = sf::Color{ 125, 125, 125 };
}

auto Input_Field::get_input_string() const -> std::string const&
{
	return input_string;
}

auto Input_Field::get_default_string() const -> std::string const&
{
	return default_string;
}

void Input_Field::set_text_color_default( sf::Color const& color )
{
	text_color_default = color;
}

void Input_Field::set_text_color_input( sf::Color const& color )
{
	text_color_input = color;
}

void Input_Field::set_input_string( std::string const& str )
{
	report_warning_if( str.size() > character_limit );

	input_string = str;

	if ( input_string.empty() )
		return;

	text_visual->fill_color = text_color_input;
	text_visual->string.set_ascii( input_string );
	text_visual->update_vertices();
}

void Input_Field::set_default_string( std::string const& str )
{
	report_warning_if( str.size() > character_limit );

	default_string = str;

	if ( not input_string.empty() or
		 default_string.empty() )
		return;

	text_visual->fill_color = text_color_default;
	text_visual->string.set_ascii( default_string );
	text_visual->update_vertices();
}

void Input_Field::input( sf::Event const& event )
{
	if ( event.type is sf::Event::EventType::MouseButtonReleased and
		 event.mouseButton.button is sf::Mouse::Button::Left ) {
		constant mouse_position = G_Window.get_mouse_position();
		constant input_bounds = text_visual->get_global_bounds();

		input_focused = rect_vs_point( input_bounds, mouse_position );
	}

	if ( not input_focused )
		return;

	input_string = text_visual->string.get_string();

	bool text_changed = false;

	if ( event.type is sf::Event::EventType::TextEntered and
		 input_string.size() <= character_limit ) {
		constant ascii = sf::String{ event.text.unicode }.toAnsiString().front();

		if ( ( ascii < 'A' or ascii > 'Z' ) and ( ascii < 'a' or ascii > 'z' ) and
			( ascii < '0' or ascii > '9' ) and ascii is_not '_' )
			return;

		input_string += ascii;
		text_changed = true;

	} else if ( event.type is sf::Event::EventType::KeyReleased ) {
		if ( event.key.code is sf::Keyboard::Key::Space and 
			 input_string.size() <= character_limit ) {
			input_string += ' ';
			text_changed = true;
		} else if ( event.key.code is sf::Keyboard::Key::BackSpace and
					not input_string.empty() ) {
			input_string.pop_back();
			text_changed = true;
		} else if ( event.key.code is sf::Keyboard::Key::Return ) {
			input_focused = false;
		}
	}

	if ( text_changed ) {
		if ( input_string.empty() )
			text_visual->string.set_ascii( default_string );
		else
			text_visual->string.set_ascii( input_string );

		text_visual->update_vertices();
		s_input_changed.emit( this );
	}
}
}