/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "clickable_label.hpp"

namespace con
{
Clickable_Label::Clickable_Label()
{
	text = attach<Rich_Text>();

	text->outline_thickness = 2.0px;
	text->set_font_from_name( "default" );
	style_for_idle();
}

auto Clickable_Label::get_status() const -> Status
{
	return status;
}

void Clickable_Label::input( sf::Event const& event )
{
	if ( event.type is_not sf::Event::MouseButtonPressed and
		 event.type is_not sf::Event::MouseMoved )
		return;

	constant mouse_in_bounds = rect_vs_point( text->get_global_bounds(), G_Window.get_mouse_position() );

	if ( mouse_in_bounds is false and
		 status is Status::Hover ) {
		style_for_idle();
		status = Status::Idle;
	} else if ( mouse_in_bounds ) {
		if ( status is Status::Idle ) {
			style_for_hover();
			status = Status::Hover;
		}

		if ( event.type is sf::Event::MouseButtonPressed and
			 event.mouseButton.button is sf::Mouse::Left )
			s_label_clicked.emit();
	}
}

void Clickable_Label::update( r32 dt )
{
	unused( dt );

	if ( text_needs_update ) {
		text->update_vertices();
		text_needs_update = false;
	}
}

void Clickable_Label::style_for_idle()
{
	text->fill_color = fill_color_idle;
	text->outline_color = outline_color_idle;

	text_needs_update = true;
}

void Clickable_Label::style_for_hover()
{
	text->fill_color = fill_color_hovered;
	text->outline_color = outline_color_hovered;

	text_needs_update = true;
}

}