/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "missile_base.hpp"

using namespace con;

// @Info: It has to be a parent of player or enemy.
// Use set_local_position to set origin of the missile.
class Missile_Shooter :
	public Node
{
public:
	template <typename TMissile>
	void set_missile_type()
	{
		static_assert( std::is_base_of_v<Missile_Base, TMissile> );
		spawning_function = [this]() -> Missile_Base& {
			return *attach( TMissile::instantiate() )->cast_to<Missile_Base>();
		};
	}

	void set_horizontal_velocity( r32 velocity );

	void shoot();

private:
	using Spawning_Function = std::function<Missile_Base&()>;

	Spawning_Function spawning_function;
	r32               horizontal_velocity;
};