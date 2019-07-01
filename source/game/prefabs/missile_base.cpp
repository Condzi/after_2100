/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "missile_base.hpp"

Missile_Base::Missile_Base()
{
	name = "unnamed_missile";

	hitbox = attach( Area::instantiate() )->cast_to<Area>();
	hitbox->name = "unnamed_missile_hitbox";

	sprite = attach( Sprite::instantiate() )->cast_to<Sprite>();
}
