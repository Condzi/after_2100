/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include <SFML/Window/Keyboard.hpp>

#include "framework/common/resources_storage.hpp"
#include "framework/scene/application.hpp"

#include "player.hpp"

Player::Player()
{
	name = "player";

	auto spr_a = Sprite::instantiate();
	spr_a->set_texture( G_Resources_Storage.get_texture( "player" ) );
	spr_a->name = "sprite_a";
	sprite_a = attach( change_owner( spr_a ) );

	auto spr_b = Sprite::instantiate();
	spr_b->name = "sprite_b";
	spr_b->set_texture( G_Resources_Storage.get_texture( "player" ) );
	sprite_b = attach( change_owner( spr_b ) );

	rotate( 90 );
	set_global_position( { 100.0px, 100.0px } );
}

void Player::update( r32 dt )
{
	check_movement_keys();
	update_illusion();
	move( velocity * dt );
	slow_down();
}

void Player::update_illusion()
{
	// Sprite that is visible on the screen.
	Node* main_sprite{ nullptr };
	Node* mirror_sprite{ nullptr };
	const Rectangle_Shape window{ {0,0}, G_App.get_window_size() };

	if ( RectVsPoint( window, sprite_a->get_global_position() ) is true ) {
		main_sprite = sprite_a;
		mirror_sprite = sprite_b;
	} else {
		main_sprite = sprite_b;
		mirror_sprite = sprite_a;
	}

	if ( main_sprite->get_global_position().y > window.size.height / 2 )
		mirror_sprite->set_global_position( { get_global_position().x, main_sprite->get_global_position().y -window.size.height } );
	else
		mirror_sprite->set_global_position( { get_global_position().x, main_sprite->get_global_position().y + window.size.height } );
}

void Player::check_movement_keys()
{
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) )
		velocity.x = -VELOCITY_MAX;
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
		velocity.x = VELOCITY_MAX;
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) )
		velocity.y = -VELOCITY_MAX;
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
		velocity.y = VELOCITY_MAX;

	// Value other than 0 is true.
	slow_horizontal = velocity.x;
	slow_vertical = velocity.y;
}

void Player::slow_down()
{
	if ( slow_horizontal )
		velocity.x *= SLOWING_MULTIPLIER;
	if ( slow_vertical )
		velocity.y *= SLOWING_MULTIPLIER;

	if ( std::fabs( velocity.x ) < 1 )
		velocity.x = 0;
	if ( std::fabs( velocity.y ) < 1 )
		velocity.y = 0;
}
