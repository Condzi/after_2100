/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "framework/scene/sprite.hpp"

using namespace con;

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
	Vec2 velocity;
	bool slow_horizontal{ true }, slow_vertical{ true };

	// For illusion (when player go up and appears from bottom)
	Node* sprite_a;
	Node* sprite_b;

	void update_illusion();
	void check_movement_keys();
	void slow_down();

};