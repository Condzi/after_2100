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

	constant sprite_size = spr_a->get_global_bounds().size;
	spr_a->set_transformation_origin( sprite_size * 0.5 );

	sprite_a = attach( change_owner( spr_a ) );

	auto spr_b = Sprite::instantiate();
	spr_b->name = "sprite_b";
	spr_b->set_texture( G_Resources_Storage.get_texture( "player" ) );
	spr_b->set_transformation_origin( sprite_size * 0.5 );
	sprite_b = attach( change_owner( spr_b ) );

	rotate( 90.0deg );

	// Sprite is rotated, so we use it height instead of width.
	sprite_a->set_local_position( { sprite_size.height / 2, 0.0px } );
	sprite_b->set_local_position( { sprite_size.height / 2, 0.0px } );

	set_global_position( { 200.0px,200.0px } );
}

void Player::update( r32 dt )
{
	check_movement_keys();
	update_illusion();
	move( velocity * dt );
	correct_for_boundary_collision();
	slow_down();
	update_tilt_transformation();
}

void Player::update_illusion()
{
	// Sprite that is visible on the screen.
	Node* main_sprite{ sprite_a };
	Node* mirror_sprite{ sprite_b };
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
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) )
		velocity.x = -VELOCITY_MAX;
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
		velocity.x = VELOCITY_MAX;
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) )
		velocity.y = -VELOCITY_MAX;
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
		velocity.y = VELOCITY_MAX;
}

void Player::slow_down()
{
	velocity *= SLOWING_MULTIPLIER;

	if ( std::fabs( velocity.x ) < 1 )
		velocity.x = 0;
	if ( std::fabs( velocity.y ) < 1 )
		velocity.y = 0;
}

void Player::correct_for_boundary_collision()
{

	constant window_width = G_App.get_window_size().width;
	constant sprite_width = sprite_a->cast_to<Sprite>()->get_global_bounds().size.width;
	constant x_pos = get_global_position().x;
	// y_pos doesn't matter since sprites has it own individual position, 
	// but it's still more clear if we set it as it was.
	constant y_pos = get_global_position().y;
	constant x_pos_max = x_pos + sprite_width;

	if ( x_pos <= 0.0px ) {
		velocity.x = 0;
		set_global_position( { 0.0px, y_pos } );
	} else if ( x_pos_max >= window_width ) {
		velocity.x = 0;
		set_global_position( { window_width - sprite_width, y_pos } );
	}
}

void Player::update_tilt_transformation()
{
	Sprite& spr_a = *sprite_a->cast_to<Sprite>();
	Sprite& spr_b = *sprite_b->cast_to<Sprite>();

	// Doesn't matter from whitch sprite we are getting the values.
	auto [pitch, yaw, roll] = spr_a.get_rotation_3d();

	if ( velocity.x != 0 )
		pitch = velocity.x * 0.12;
	if ( velocity.y != 0 )
		yaw = velocity.y * 0.12;

	// @Bugfix: When x and y > 0 then yaw is pointed in wrong direction.
	// This solves the problem.
	//if ( velocity.x > 0 and velocity.y > 0 )
	//	pitch *= -1;

	spr_a.set_rotation_3d( pitch, yaw, roll );
	spr_b.set_rotation_3d( pitch, yaw, roll );
}
