/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "framework/scene/node.hpp"
#include "framework/scene/sprite.hpp"
#include "framework/scene/area.hpp"

using namespace con;

// @Idea: Missile as a path follower
class Missile_Base :
	public Node
{
	CLASS_DEF( Missile_Base );

public:
	Area*   hitbox;
	Sprite* sprite;
	Vec2    velocity;

	Missile_Base();
};
