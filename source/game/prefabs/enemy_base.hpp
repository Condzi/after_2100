/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "framework/scene/path.hpp"
#include "framework/scene/sprite.hpp"
#include "framework/scene/area.hpp"

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
	Area*   hitbox;

private:
	static constexpr r32 TILT_MULTIPLIER = 0.18;

	void update_tilt_transformation();
};