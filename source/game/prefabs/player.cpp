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

	sprite_a = attach<Sprite>();
	sprite_a->name = "sprite_a";
	sprite_a->set_texture_from_name( "player" );
	sprite_a->layer = 3;

	constant sprite_size = sprite_a->get_global_bounds().size;
	// After rotating by 90deg width gets swapped with height.
	sprite_a->set_local_position( { sprite_size.height / 2, 0.0px } );
	sprite_a->set_transformation_origin( sprite_size * 0.5 );

	rotate( 90.0deg );

	gun_a_1 = sprite_a->attach<Missile_Shooter>();
	gun_a_2 = sprite_a->attach<Missile_Shooter>();

	gun_a_1->name = "gun_a_1";
	gun_a_2->name = "gun_a_2";


	gun_a_1->set_missile_type<Player_Missile>();
	gun_a_2->set_missile_type<Player_Missile>();


	gun_a_1->set_horizontal_velocity( 500 );
	gun_a_2->set_horizontal_velocity( 500 );
;

	gun_a_1->set_local_position( Size2{ 50.0px, -36.0px } );
	gun_a_2->set_local_position( Size2{ 50.0px, 36.0px } );

	gun_a_1->set_cooldown_time( 0.2sec );
	gun_a_2->set_cooldown_time( 0.2sec );


	hitbox_a = sprite_a->attach<Area>();

	hitbox_a->name = "hitbox_" + name;

	health = attach<Health>();
	health->set_max( 10 );
	set_absolute_position( Percent_Position{ 0, 50 } );
}

void Player::update( r32 dt )
{
	G_Profile_Scope( "Player::update" );
	G_Audio_Listener.set_position( sprite_a->get_global_position() );

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
		 gun_a_1->is_ready_to_shoot() ) {

		gun_a_1->shoot();
		gun_a_2->shoot();

		// @ToDo: Separate variable for recoil? According to Newton's law (am = -am)
		// we need some mass multiplier.
		constant recoil = gun_a_1->get_horizontal_velocity() * 0.8;
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

	constant window_width = G_Window.get_size().width;
	constant sprite_width = sprite_a->get_global_bounds().size.width;
	constant x_pos = get_global_position().x;
	constant x_pos_max = x_pos + sprite_width;
	// y_pos doesn't matter since sprites has it own individual position,
	// but it's still more clear if we set it as it was.
	constant y_pos = get_global_position().y;

	if ( x_pos < 0.0px ) {
		set_global_position( { 0.0px, y_pos } );
		if ( acceleration_direction.x is -1 )
			acceleration_direction.x = 0;
	} else if ( x_pos_max > window_width ) {
		set_global_position( { window_width - sprite_width, y_pos } );
		if ( acceleration_direction.x is 1 )
			acceleration_direction.x = 0;
	}
}

void Player::update_tilt_transformation()
{
	G_Profile_Function();

	// Doesn't matter from whitch sprite we are getting the values.
	auto [pitch, yaw, roll] = sprite_a->get_rotation_3d();

	pitch = velocity.x * TILT_MULTIPLIER;
	yaw = velocity.y * TILT_MULTIPLIER;

	sprite_a->set_rotation_3d( pitch, yaw, roll );

	hitbox_a->set_rectangle_shape( sprite_a->get_global_bounds() );
}