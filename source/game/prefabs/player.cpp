/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/audio_listener.hpp"
#include "framework/common/resources_storage.hpp"
#include "framework/scene/application.hpp"

#include "player.hpp"

#include <SFML/Window/Keyboard.hpp>

Player::Player()
{
	name = "player";

	sprite = attach<Sprite>();
	sprite->name = "sprite";
	sprite->set_texture_from_name( "player" );
	sprite->layer = 3;

	constant sprite_size = sprite->get_global_bounds().size;
	// After rotating by 90deg width gets swapped with height.
	sprite->set_local_position( { sprite_size.height / 2, sprite_size.width / 2 } );
	sprite->set_transformation_origin( sprite_size * 0.5 );

	rotate( 90.0deg );

	gun_a = sprite->attach<Missile_Shooter>();
	gun_b = sprite->attach<Missile_Shooter>();

	gun_a->name = "gun_a";
	gun_b->name = "gun_b";

	gun_a->set_missile_type<Player_Missile>();
	gun_b->set_missile_type<Player_Missile>();

	gun_a->set_horizontal_velocity( 500 );
	gun_b->set_horizontal_velocity( 500 );

	gun_a->set_local_position( Size2{ 50.0px, -36.0px } );
	gun_b->set_local_position( Size2{ 50.0px, 36.0px } );

	gun_a->set_cooldown_time( 0.2sec );
	gun_b->set_cooldown_time( 0.2sec );

	hitbox = sprite->attach<Area>();

	hitbox->name = "hitbox_" + name;

	health = attach<Health>();
	health->set_max( 10 );
	set_absolute_position( Percent_Position{ 0, 45 } );
}

void Player::update( r32 dt )
{
	G_Audio_Listener.set_position( get_global_position() );

	check_movement_keys();
	move( velocity * dt );
	correct_for_boundary_collision();
	slow_down();
	accelerate( dt );
	update_tilt_transformation();
}

void Player::check_movement_keys()
{
	G_Profile_Function();

	if ( G_Window.is_focused() is false )
		return;

	acceleration_direction = Vec2::Zero();

	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) and velocity.x > -VELOCITY_MAX )
		acceleration_direction += Vec2::Left();
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) and velocity.x < VELOCITY_MAX )
		acceleration_direction += Vec2::Right();
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) and velocity.y > -VELOCITY_MAX )
		acceleration_direction += Vec2::Up();
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) and velocity.y < VELOCITY_MAX )
		acceleration_direction += Vec2::Down();

	// Should be in own method?
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) and
		 gun_a->is_ready_to_shoot() ) {

		gun_a->shoot();
		gun_b->shoot();

		// @ToDo: Separate variable for recoil? According to Newton's law (am = -am)
		// we need some mass multiplier.
		constant recoil = gun_a->get_horizontal_velocity() * 0.8;
		velocity.x -= recoil;
	}
}

void Player::slow_down()
{
	G_Profile_Function();

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
}

void Player::correct_for_boundary_collision()
{
	G_Profile_Function();

	constant window_size = G_Window.get_size();
	constant sprite_size = sprite->get_global_bounds().size;

	constant x_pos     = get_global_position().x;
	constant x_pos_max = x_pos + sprite_size.width;
	constant y_pos     = get_global_position().y;
	constant y_pos_max = y_pos + sprite_size.height;

	auto new_position = get_global_position();

	if ( x_pos < 0 ) {
		new_position.x = 0;

		if ( acceleration_direction.x is -1 )
			acceleration_direction.x = 0;
	} else if ( x_pos_max > window_size.width ) {
		new_position.x = window_size.width - sprite_size.width;

		if ( acceleration_direction.x is 1 )
			acceleration_direction.x = 0;
	}

	if ( y_pos < 0 ) {
		new_position.y = 0;
		if ( acceleration_direction.y is -1 )
			acceleration_direction.y = 0;
	} else if ( y_pos_max > window_size.height ) {
		new_position.y = window_size.height - sprite_size.height;

		if ( acceleration_direction.y is 1 )
			acceleration_direction.y = 0;
	}

	set_global_position( new_position );
}

void Player::update_tilt_transformation()
{
	G_Profile_Function();

	// Doesn't matter from whitch sprite we are getting the values.
	auto [pitch, yaw, roll] = sprite->get_rotation_3d();

	pitch = velocity.x * TILT_MULTIPLIER;
	yaw = velocity.y * TILT_MULTIPLIER;

	sprite->set_rotation_3d( pitch, yaw, roll );

	hitbox->set_rectangle_shape( sprite->get_global_bounds() );
}