/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "missile_base.hpp"

Missile_Base::Missile_Base()
{
	name = "unnamed_missile";
	// We don't want to move missile with parent.
	// (Missile is child of Missile_Spawner, which is child of Player or Enemy)
	move_with_parent = false;

	hitbox = attach<Area>();
	hitbox->name = "hitbox_" + name;
}