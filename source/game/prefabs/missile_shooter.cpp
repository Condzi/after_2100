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

void Missile_Shooter::set_cooldown_time( r32 seconds )
{
	cooldown_time = sf::seconds( seconds );
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
	report_warning_if( !spawning_function )
	{
		log_warning( "No spawning_function set for Missile_Shooter '{}', child of '{}'", name, get_parent()->name );
		return;
	}

	report_warning_if( horizontal_velocity is 0 )
	{
		log_warning( "No horizontal_velocity set for Missile_Shooter '{}', child of '{}'", name, get_parent()->name );
		return;
	}

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
	if ( time_to_next_shot.asSeconds() - delta <= 0 ) {
		time_to_next_shot = sf::Time::Zero;
		is_ready_to_shoot = true;
	} else
		time_to_next_shot -= sf::seconds( delta );
}

void Missile_Shooter::draw( Drawing_Set& drawing_set )
{
	if ( G_Debug_Flags.draw_missile_shooters is false )
		return;

	visual_representation.setPosition( get_global_position() );
	visual_representation.setFillColor( sf::Color::Transparent );
	visual_representation.setOutlineColor( debug_circle_color );
	visual_representation.setOutlineThickness( -2.0px );
	visual_representation.setRadius( debug_circle_radious );

	drawing_set.add_drawable( visual_representation, debug_circle_layer );
}
