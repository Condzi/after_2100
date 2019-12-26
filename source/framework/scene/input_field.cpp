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
	temporary_string = input_string;

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

		constant input_was_focused = input_focused;
		input_focused = rect_vs_point( input_bounds, mouse_position );

		// To erase the default value when clicking.
		if ( input_focused and input_string.empty() ) {
			text_visual->string.set_ascii( "" );
			text_visual->update_vertices();
		}

		if ( input_focused )
			temporary_string = input_string;

		if ( input_was_focused and not input_focused ) {
			if ( input_string.empty() ) {
				text_visual->string.set_ascii( default_string );
				text_visual->fill_color = text_color_default;
			} else {
				text_visual->string.set_ascii( input_string );
				text_visual->fill_color = text_color_input;
			}
			text_visual->update_vertices();
		}
	}

	if ( not input_focused )
		return;

	bool text_changed = false;

	if ( event.type is sf::Event::EventType::TextEntered and
		 temporary_string.size() < character_limit ) {
		constant ascii = sf::String{ event.text.unicode }.toAnsiString().front();

		if ( ( ascii < 'A' or ascii > 'Z' ) and ( ascii < 'a' or ascii > 'z' ) and
			( ascii < '0' or ascii > '9' ) and ascii is_not '_' )
			return;

		temporary_string += ascii;
		text_changed = true;

	} else if ( event.type is sf::Event::EventType::KeyReleased ) {
		if ( event.key.code is sf::Keyboard::Key::Space and
			 temporary_string.size() < character_limit ) {
			temporary_string += ' ';
			text_changed = true;

		} else if ( event.key.code is sf::Keyboard::Key::BackSpace and
					not temporary_string.empty() ) {
			temporary_string.pop_back();
			text_changed = true;

		} else if ( event.key.code is sf::Keyboard::Key::Return ) {
			input_focused = false;
			input_string = temporary_string;
			s_input_changed.emit( this );

			if ( input_string.empty() ) {
				text_visual->string.set_ascii( default_string );
				text_visual->fill_color = text_color_default;
			} else {
				text_visual->string.set_ascii( input_string );
				text_visual->fill_color = text_color_input;
			}

			text_visual->update_vertices();
		}
	}

	if ( text_changed ) {
		text_visual->string.set_ascii( temporary_string );
		text_visual->fill_color = text_color_input;

		text_visual->update_vertices();
	}
}
}