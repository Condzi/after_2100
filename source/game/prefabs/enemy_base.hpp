/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "framework/scene/path.hpp"

using namespace con;

class Enemy_Base :
	public Path_Follower
{
	CLASS_DEF( Enemy_Base );

public:
	Enemy_Base();

	void update( r32 dt ) override;

private:
	static constexpr r32 TILT_MULTIPLIER = 0.18;

	void update_tilt_transformation();
};