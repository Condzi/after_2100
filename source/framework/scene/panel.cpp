/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "panel.hpp"

namespace con
{
void Panel::set_size( Size2 const& size )
{
	visual.setSize( size );
}

void Panel::set_color( sf::Color const& color )
{
	visual.setFillColor( color );
}

auto Panel::get_size() const -> Size2
{
	return visual.getSize();
}

auto Panel::get_color() const -> sf::Color
{
	return visual.getFillColor();
}

void Panel::update( r32 dt )
{
	unused( dt );

	visual.setPosition( get_global_position() );
	visual.setScale( get_scale() );
	visual.setRotation( get_rotation() );
}

void Panel::draw_gui( Drawing_Set& set )
{
	if ( visible )
		set.add_drawable( visual, layer );
}
}