/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "framework/scene/sprite.hpp"

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
	static constexpr r32 VELOCITY_MAX = 200;
	static constexpr r32 SLOWING_MULTIPLIER = 0.86;
	static constexpr r32 ACCELERATION_MULTIPLIER = 0.1;
	static constexpr r32 TILT_MULTIPLIER = 0.09;

	Vec2 acceleration_direction;
	Vec2 velocity;

	// For illusion (when player go up and appears from bottom)
	Node* sprite_a;
	Node* sprite_b;

	void update_illusion();
	void check_movement_keys();
	void slow_down();
	void accelerate( r32 dt );
	void correct_for_boundary_collision();
	void update_tilt_transformation();

};