/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "health.hpp"

#include <SFML/System/Time.hpp>

Health::Health()
{
	name = "health";
}

auto Health::get_max() const -> s32
{
	return max_hp;
}

auto Health::get_current() const -> s32
{
	return current_hp;
}

void Health::set_max( s32 max_value )
{
	report_error_if( max_value <= 0 )
	{
		return;
	}

	max_hp = max_value;

	if ( current_hp > max_hp )
		current_hp = max_hp;
}

bool Health::is_dead() const
{
	return current_hp is 0;
}

void Health::heal( s32 value, Node* healer )
{
	s32 health_to_add = value;

	if ( current_hp + value > max_hp )
		health_to_add = max_hp - current_hp;

	current_hp += health_to_add;

	// idk if we should pass value or health_to_add?
	s_heal.notify( value, healer );
}

void Health::damage( s32 value, Node* dealer )
{
	if ( is_dead() )
		return;

	current_hp -= value;
	s_damage.notify( value, dealer );

	if ( current_hp < 0 ) {
		current_hp = 0;
		s_dead.notify();
	}
}

void Health::refill()
{
	s_refill.notify();

	current_hp = max_hp;
}
