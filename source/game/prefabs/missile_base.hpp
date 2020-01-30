/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/
#pragma once

#include "framework/scene/area.hpp"
#include "framework/scene/node.hpp"

using namespace con;

// @Idea: Missile as a path follower
class Missile_Base :
	public Node
{
	CLASS_DEF( Missile_Base );

public:
	Area* hitbox;
	Vec2  velocity;

	Missile_Base();
};