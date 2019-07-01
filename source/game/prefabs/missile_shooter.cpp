/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "missile_shooter.hpp"

void Missile_Shooter::set_horizontal_velocity( r32 velocity )
{
	horizontal_velocity = velocity;
}

void Missile_Shooter::shoot()
{
	Missile_Base& new_missile{ spawning_function() };

	new_missile.velocity.x = horizontal_velocity;
	new_missile.set_global_position( get_global_position() );
}
