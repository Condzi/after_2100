/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/assertions.hpp"
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
		report_warning_if( drawing_set.add_drawable( sprite, layer ) returned false );
}

void Sprite::set_texture( sf::Texture const* texture )
{
	if ( texture is nullptr )
		return;

	sprite.setTexture( *texture );
}

auto Sprite::get_global_bounds() const -> Rectangle_Shape
{
	return sprite.getGlobalBounds();
}

auto Sprite::get_texture() const -> sf::Texture const*
{
	return sprite.getTexture();
}
}