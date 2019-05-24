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

	sprite_a = attach( Sprite::instantiate() )->cast_to<Sprite>();
	sprite_b = attach( Sprite::instantiate() )->cast_to<Sprite>();
	sprite_a->name = "sprite_a";
	sprite_b->name = "sprite_b";
	sprite_a->set_texture( G_Resources_Storage.get_texture( "player" ) );
	sprite_b->set_texture( G_Resources_Storage.get_texture( "player" ) );

	constant sprite_size = sprite_a->get_global_bounds().size;
	// After rotating by 90deg width gets swapped with height.
	sprite_a->set_local_position( { sprite_size.height / 2, 0.0px } );
	sprite_b->set_local_position( { sprite_size.height / 2, 0.0px } );

	sprite_a->set_transformation_origin( sprite_size * 0.5 );
	sprite_b->set_transformation_origin( sprite_size * 0.5 );

	rotate( 90.0deg );

	set_global_position( { 200.0px,200.0px } );
}

void Player::update( r32 dt )
{
	check_movement_keys();
	move( velocity * dt );
	correct_for_boundary_collision();
	update_illusion();
	slow_down();
	accelerate( dt );
	update_tilt_transformation();
}

void Player::update_illusion()
{
	// Sprite that is visible on the screen.
	Sprite* main_sprite{ sprite_a };
	Sprite* mirror_sprite{ sprite_b };
	const Rectangle_Shape window{ {0,0}, G_App.get_window_size() };

	if ( RectVsPoint( window, sprite_b->get_global_position() ) is true )
		std::swap( main_sprite, mirror_sprite );

	Vec2 pos_to_set = main_sprite->get_global_position();

	if ( main_sprite->get_global_position().y > window.size.height / 2 )
		pos_to_set.y -= window.size.height;
	else
		pos_to_set.y += window.size.height;

	mirror_sprite->set_global_position( pos_to_set );
}

void Player::check_movement_keys()
{
	acceleration_direction = Vec2::ZERO();

	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) )
		acceleration_direction += Vec2::LEFT();
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
		acceleration_direction += Vec2::RIGHT();
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) )
		acceleration_direction += Vec2::UP();
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
		acceleration_direction += Vec2::DOWN();
}

void Player::slow_down()
{
	if ( acceleration_direction.x is 0 ) {
		velocity.x *= SLOWING_MULTIPLIER;
		if ( std::fabs( velocity.x ) < 1 )
			velocity.x = 0;
	}
	if ( acceleration_direction.y is 0 ) {
		velocity.y *= SLOWING_MULTIPLIER;
		if ( std::fabs( velocity.y ) < 1 )
			velocity.y = 0;
	}
}

void Player::accelerate( r32 dt )
{
	velocity += acceleration_direction * VELOCITY_MAX * ACCELERATION_MULTIPLIER;

	if ( std::fabs( velocity.x ) > VELOCITY_MAX )
		velocity.x = VELOCITY_MAX * acceleration_direction.x;

	if ( std::fabs( velocity.y ) > VELOCITY_MAX )
		velocity.y = VELOCITY_MAX * acceleration_direction.y;
}

void Player::correct_for_boundary_collision()
{
	constant window_width = G_App.get_window_size().width;
	constant sprite_width = sprite_a->get_global_bounds().size.width;
	constant x_pos = get_global_position().x;
	constant x_pos_max = x_pos + sprite_width;
	// y_pos doesn't matter since sprites has it own individual position, 
	// but it's still more clear if we set it as it was.
	constant y_pos = get_global_position().y;

	if ( x_pos <= 0.0px )
		set_global_position( { 0.0px, y_pos } );
	else if ( x_pos_max >= window_width )
		set_global_position( { window_width - sprite_width, y_pos } );
}

void Player::update_tilt_transformation()
{
	// Doesn't matter from whitch sprite we are getting the values.
	auto [pitch, yaw, roll] = sprite_a->get_rotation_3d();

	pitch = velocity.x * TILT_MULTIPLIER;
	yaw = velocity.y * TILT_MULTIPLIER;

	sprite_a->set_rotation_3d( pitch, yaw, roll );
	sprite_b->set_rotation_3d( pitch, yaw, roll );
}
