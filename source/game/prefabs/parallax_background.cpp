/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/resources_storage.hpp"
#include "framework/scene/camera.hpp"

#include "parallax_background.hpp"

#include <SFML/Graphics/Texture.hpp>

Parallax_Background::Parallax_Background()
{
	sprite_a = attach<Sprite>();
	sprite_b = attach<Sprite>();

	sprite_a->name = "sprite_a";
	sprite_b->name = "sprite_b";
}

void Parallax_Background::set_texture_from_pointer( sf::Texture const* texture )
{
	if ( texture is nullptr )
		return;

	sprite_a->set_texture_from_pointer( texture );
	sprite_b->set_texture_from_pointer( texture );

	sprite_a->set_global_position( get_global_position() );
	sprite_b->set_global_position( get_global_position() );

	sprite_b->move( { cast<r32>( texture->getSize().x ), 0 } );
}

void Parallax_Background::set_texture_from_name( std::string const& name )
{
	set_texture_from_pointer( G_Resources_Storage.get_texture( name ) );
}

void Parallax_Background::update( r32 delta )
{
	sprite_a->visible = sprite_b->visible = visible;
	sprite_a->layer   = sprite_b->layer = layer;

	parallax_move( delta );

	correct_sprite_positions();
}

void Parallax_Background::parallax_move( r32 const dt )
{
	report_error_if( speed < 0 );

	constant shift_vector = Vec2{ -speed*dt, 0 };
	sprite_a->move( shift_vector );
	sprite_b->move( shift_vector );
}

void Parallax_Background::correct_sprite_positions()
{
	Sprite* left_sprite  = sprite_a;
	Sprite* right_sprite = sprite_b;

	if ( right_sprite->get_global_position().x < left_sprite->get_global_position().x )
		std::swap( left_sprite, right_sprite );

	constant camera_bounds       = get_node( "root/game_camera" )->cast_to<Camera>()->get_global_bounds();
	constant left_sprite_bounds  = left_sprite->get_global_bounds();
	constant right_sprite_bounds = right_sprite->get_global_bounds();

	if ( left_sprite_bounds.position.x + left_sprite_bounds.size.width < camera_bounds.position.x )
		left_sprite->set_global_position( { right_sprite_bounds.position.x + right_sprite_bounds.size.width, left_sprite_bounds.position.y } );
}