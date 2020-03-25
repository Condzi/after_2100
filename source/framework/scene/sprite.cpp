/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/assertions.hpp"
#include "framework/common/resources_storage.hpp"

#include "sprite.hpp"

namespace con
{
void Sprite::update( r32 delta )
{
	unused( delta );

	sprite.setPosition( get_global_position() );
	sprite.setRotation( get_rotation() );
	sprite.setScale( get_scale() );
}

void Sprite::draw( Drawing_Set& drawing_set )
{
	if ( visible )
		drawing_set.add_drawable( sprite, layer );
}

void Sprite::set_texture_from_pointer( sf::Texture const* texture )
{
	if ( texture is nullptr )
		return;

	sprite.setTexture( *texture );
}

void Sprite::set_texture_from_name( std::string const& name )
{
	set_texture_from_pointer( G_Resources_Storage.get_texture( name ) );
}

void Sprite::set_rotation_3d( r32 pitch, r32 yaw, r32 roll )
{
	sprite.setRotation3d( { pitch, yaw, roll } );
}

void Sprite::set_transformation_origin( Point const& point )
{
	sprite.setOrigin( point );
}

void Sprite::set_texture_rect( Rectangle_Shape const& rect )
{
	sprite.setTextureRect( rect );
}

void Sprite::set_fill_color( sf::Color const& color )
{
	sprite.setColor( color );
}

auto Sprite::get_global_bounds() const -> Rectangle_Shape
{
	// Fix for a bug.
	// Sprite has correct scale, size and position but Sprite3d has not been updated,
	// so we force it to update anyway .
	const_cast<Sprite&>( *this ).update( 0.0 );

	return sprite.getGlobalBounds();
}

auto Sprite::get_texture() const -> sf::Texture const*
{
	return sprite.getTexture();
}

auto Sprite::get_rotation_3d() const -> Rotation3D
{
	constant[pitch, yaw, roll] = sprite.getRotation3d();

	return { pitch, yaw, roll };
}

auto Sprite::get_sprite_raw() -> sf::Sprite3d&
{
	return sprite;
}

auto Sprite::get_transformation_origin() const -> Point
{
	return sprite.getOrigin();
}

auto Sprite::get_texture_rect() -> Rectangle_Shape
{
	return sprite.getTextureRect();
}
}