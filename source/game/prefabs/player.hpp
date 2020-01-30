/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#pragma once

#include "framework/scene/area.hpp"
#include "framework/scene/sprite.hpp"
#include "framework/scene/sound_source.hpp"

#include "exploded_sprite.hpp"
#include "explosion.hpp"

#include "health.hpp"
#include "missile_shooter.hpp"
#include "player_missile.hpp"

using namespace con;

class Player final :
	public Node
{
	CLASS_DEF( Player );

public:
	Player();

	void update( r32 dt ) override;

private:
	compile_constant VELOCITY_MAX = 280.0f;
	compile_constant SLOWING_MULTIPLIER = 0.92f;
	compile_constant ACCELERATION_MULTIPLIER = 0.05f;
	compile_constant TILT_MULTIPLIER = 0.09f;

	Vec2 acceleration_direction;
	Vec2 velocity;

	Sprite* sprite;
	// Children of sprite
	Missile_Shooter* gun_a;
	Missile_Shooter* gun_b;
	Area* hitbox;

	Explosion* explosion;
	Exploded_Sprite* exploded_sprite;

	Health* health;
	Sound_Source* low_health_sound;

	void check_movement_keys();
	void slow_down();
	void accelerate( r32 dt );
	void correct_for_boundary_collision();
	void update_tilt_transformation();
};