/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "checkbox.hpp"

#include <SFML/Window/Mouse.hpp>

namespace con
{

Checkbox::Checkbox()
{
	fill_checked = sf::Color( { 100, 100, 100 } );
	fill_unchecked = sf::Color( { 210, 210, 210 } );

	set_outline_thickness( 2 );
	set_size( { 16, 16 } );
	set_outline_color( sf::Color{ 50, 50,50 } );
}

void Checkbox::set_size( Size2 const& size )
{
	visual.setSize( size );
}

void Checkbox::set_fill_color( sf::Color const& color )
{
	visual.setFillColor( color );
}

void Checkbox::set_outline_color( sf::Color const& color )
{
	visual.setOutlineColor( color );
}

void Checkbox::set_outline_thickness( r32 thickness )
{
	visual.setOutlineThickness( thickness );
}

auto Checkbox::get_size() const -> Size2
{
	return visual.getSize();
}

auto Checkbox::get_fill_color_checked() const -> sf::Color
{
	return fill_checked;
}

auto Checkbox::get_fill_color_unchecked() const -> sf::Color
{
	return fill_unchecked;
}

auto Checkbox::get_outline_color() const -> sf::Color
{
	return visual.getOutlineColor();
}

auto Checkbox::get_outline_thickness() const -> r32
{
	return visual.getOutlineThickness();
}

void Checkbox::input( sf::Event const& event )
{
	constant mouse_position = G_Window.get_mouse_position();
	constant checkbox_rect = Rectangle_Shape{ get_global_position(), get_size() };

	if ( event.type is sf::Event::EventType::MouseButtonReleased and
		 event.mouseButton.button is sf::Mouse::Left and
		 rect_vs_point( checkbox_rect, mouse_position ) returned true ) {
			 
		checked = !checked;
		s_toggle.emit( checked );
	}
}

void Checkbox::update( r32 dt )
{
	unused( dt );

	visual.setPosition( get_global_position() );
	visual.setScale( get_scale() );
	visual.setRotation( get_rotation() );
	visual.setFillColor( checked ? fill_checked : fill_unchecked );
}

void Checkbox::draw_gui( Drawing_Set& set )
{
	set.add_drawable( visual, layer );
}

}