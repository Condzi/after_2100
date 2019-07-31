/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "label.hpp"

#include "framework/common/resources_storage.hpp"
#include "framework/localization/locale.hpp"

namespace con
{
Label::Label()
{
	text.setPosition( get_global_position() );
	text.setRotation( get_rotation() );
	text.setScale( get_scale() );
}

void Label::set_font( std::string const& name )
{
	text.setFont( *G_Resources_Storage.get_font( name ) );
}

void Label::set_character_size( size_t size )
{
	text.setCharacterSize( size );
}

void Label::set_fill_color( sf::Color const& color )
{
	text.setFillColor( color );
}

void Label::set_outline_color( sf::Color const& color )
{
	text.setOutlineColor( color );
}

void Label::set_outline_thickness( r32 thickness )
{
	text.setOutlineThickness( thickness );
}

auto Label::get_character_size() const -> size_t
{
	return text.getCharacterSize();
}

auto Label::get_global_bounds() const -> Rectangle_Shape
{
	return text.getGlobalBounds();
}

void Label::draw_gui( Drawing_Set& set )
{
	if ( not visible or 
		 text.getString().isEmpty() )
		return;

	if( text.getFont() is nullptr )
		text.setFont( *G_Resources_Storage.get_font( "default" ) );

	set.add_drawable( text, layer );
}

void Label::update( r32 dt )
{
	unused( dt );

	text.setPosition( get_global_position() );
	text.setRotation( get_rotation() );
	text.setScale( get_scale() );

	text.setString( string.get_string() );
}
}