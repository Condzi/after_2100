/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "framework/scene/animation.hpp"

#include "missile_base.hpp"

using namespace con;

class Player_Missile :
	public Missile_Base
{
	CLASS_DEF( Player_Missile );

public:
	Player_Missile();

	void update( r32 delta ) override;

private:
	Animation* missile_animation;
};