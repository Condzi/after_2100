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

void Missile_Shooter::set_cooldown_time( sf::Time const& time )
{
	cooldown_time = time;
}

auto Missile_Shooter::get_cooldown_time() const -> sf::Time const&
{
	return cooldown_time;
}

auto Missile_Shooter::get_time_to_next_shot() const -> sf::Time const&
{
	return time_to_next_shot;
}

bool Missile_Shooter::get_is_ready_to_shoot() const
{
	return is_ready_to_shoot;
}

void Missile_Shooter::shoot()
{
	if ( is_ready_to_shoot is false )
		return;

	Missile_Base& new_missile{ spawning_function() };

	new_missile.velocity.x = horizontal_velocity;
	new_missile.set_global_position( get_global_position() );

	time_to_next_shot = cooldown_time;
	is_ready_to_shoot = false;
}

void Missile_Shooter::update( r32 delta )
{
	if ( time_to_next_shot.asSeconds() - delta <= 0  ) {
		time_to_next_shot = sf::Time::Zero;
		is_ready_to_shoot = true;
	}
}
