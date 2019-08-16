/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "missile_base.hpp"

#include <SFML/System/Time.hpp>

using namespace con;

// @Info: It has to be a parent of player or enemy.
// Use set_local_position to set origin of the missile.
class Missile_Shooter final :
	public Node
{
	CLASS_DEF( Missile_Shooter );

public:
	sf::Color debug_circle_color{ sf::Color::Green };
	r32       debug_circle_radious{ 10.00px };
	s32       debug_circle_layer{ 50 };

	template <typename TMissile>
	void set_missile_type()
	{
		static_assert( std::is_base_of_v<Missile_Base, TMissile> );
		spawning_function = [this]() -> Missile_Base& {
			return *attach<TMissile>();
		};
	}

	void set_horizontal_velocity( r32 velocity );
	void set_cooldown_time( r32 seconds );

	[[nodiscard]] auto get_cooldown_time() const       -> sf::Time const&;
	[[nodiscard]] auto get_time_to_next_shot() const   -> sf::Time const&;
	[[nodiscard]] auto is_ready_to_shoot() const       -> bool;
	[[nodiscard]] auto get_horizontal_velocity() const -> r32;

	void shoot();

	void update( r32 delta ) override;
	void draw( Drawing_Set& drawing_set ) override;

private:
	using Spawning_Function = std::function<Missile_Base& ( )>;

	Spawning_Function spawning_function;
	r32               horizontal_velocity;
	sf::Time          cooldown_time;
	sf::Time          time_to_next_shot;
	bool              ready_to_shoot{ true };

	sf::CircleShape visual_representation;
};