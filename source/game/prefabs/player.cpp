/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include <SFML/Window/Keyboard.hpp>

#include "framework/common/resources_storage.hpp"

#include "player.hpp"

Player::Player()
{
	name = "player";
	set_texture( G_Resources_Storage.get_texture( "player" ) );
	rotate( 90 );
}

void Player::update( r32 dt )
{
	Sprite::update( dt );

	check_movement_keys();
	move( velocity * dt );
	slow_down();
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

	if ( std::abs( velocity.x ) < 1 )
		velocity.x = 0;
	if ( std::abs( velocity.y ) < 1 )
		velocity.y = 0;
}
