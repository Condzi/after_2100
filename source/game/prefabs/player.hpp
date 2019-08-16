/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "framework/scene/sprite.hpp"
#include "framework/scene/area.hpp"
#include "health.hpp"
#include "player_missile.hpp"
#include "missile_shooter.hpp"

using namespace con;

// @Info: Because of the illusion - the player position is split in two: for sprite_a and sprite_b.
// It's main position (get_global_position.y) should not be used.
class Player final :
	public Node
{
	CLASS_DEF( Player );

public:
	Player();

	void update( r32 dt ) override;

private:
	compile_constant VELOCITY_MAX = 280.0f;
	compile_constant SLOWING_MULTIPLIER = 0.86f;
	compile_constant ACCELERATION_MULTIPLIER = 0.1f;
	compile_constant TILT_MULTIPLIER = 0.09f;

	Vec2 acceleration_direction;
	Vec2 velocity;

	// For illusion (when player go up and appears from bottom)
	Sprite* sprite_a;
	Sprite* sprite_b;

	// Children of sprite_a and _b
	Missile_Shooter* gun_a_1;
	Missile_Shooter* gun_a_2;
	Missile_Shooter* gun_b_1;
	Missile_Shooter* gun_b_2;
	
	// Children of sprites.
	Area* hitbox_a;
	Area* hitbox_b;

	Health* health;

	void update_illusion();
	void check_movement_keys();
	void slow_down();
	void accelerate( r32 dt );
	void correct_for_boundary_collision();
	void update_tilt_transformation();
};