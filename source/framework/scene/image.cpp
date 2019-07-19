/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "image.hpp"

namespace con
{
void Image::update( r32 dt )
{
	unused( dt );

	sprite.setPosition( get_global_position() );
	sprite.setRotation( get_rotation() );
	sprite.setScale( get_scale() );
}

void Image::draw_gui( Drawing_Set& set )
{
	set.add_drawable( sprite, layer );
}

void Image::set_texture( sf::Texture const* texture )
{
	if ( texture is nullptr )
		return;

	sprite.setTexture( *texture );
}

void Image::set_transformation_origin( Point const& point )
{
	sprite.setOrigin( point );
}

void Image::set_texture_rect( Rectangle_Shape const& rect )
{
	sprite.setTextureRect( rect );
}

auto Image::get_global_bounds() const -> Rectangle_Shape
{
	// Fix for a bug.
	// Sprite has correct scale, size and position but Sprite3d has not been updated,
	// so we force it to update anyway .
	const_cast<Image&>( *this ).update( 0.0 );

	return sprite.getGlobalBounds();
}

auto Image::get_texture() const -> sf::Texture const*
{
	return sprite.getTexture();
}

auto Image::get_sprite_raw() -> sf::Sprite &
{
	return sprite;
}

auto Image::get_transformation_origin() const -> Point
{
	return sprite.getOrigin();
}

auto Image::get_texture_rect() -> Rectangle_Shape
{
	return sprite.getTextureRect();
}
}