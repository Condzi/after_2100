/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "framework/scene/path.hpp"
#include "framework/scene/sprite.hpp"
#include "framework/scene/area.hpp"

#include "explosion.hpp"
#include "health.hpp"
#include "exploded_sprite.hpp"

using namespace con;

class Enemy_Base :
	public Path_Follower
{
	CLASS_DEF( Enemy_Base );

public:
	Enemy_Base();

	void update( r32 dt ) override;

protected:
	Sprite* sprite;
	Area* hitbox;
	Health* health;
	Explosion* explosion;
	Exploded_Sprite* exploded_sprite;

private:
	compile_constant TILT_MULTIPLIER = 0.18f;

	void update_tilt_transformation();
};