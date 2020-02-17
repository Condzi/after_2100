/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/
#pragma once

#include "node.hpp"

namespace con
{
class Particle_Emitter :
	public Node
{
	CLASS_DEF( Particle_Emitter );

private:
	struct Particle final	
	{
		Vec2 position;
		sf::Color color;
		Vec2 velocity;
		r32 remaining_lifetime;
		r32 rotation;
	};

public:
	struct
	{
		sf::Texture const* texture{ nullptr };
		Rectangle_Shape    texture_rect;
		sf::Color          color{ sf::Color::White };

		r32 lifetime{ 1.0sec }; // 0 - unlimited

		r32  spin_velocity{ 0 }; // in angles
		r32  initial_velocity{ 100.0f };
		Vec2 force_to_apply; // Gravity etc.

		bool one_shot{ false }; // release all particles once.
	} settings;

	void set_particles_count( s32 count );

	auto get_particles_count() const -> s32;

	void update( r32 dt ) override;
	void draw( Drawing_Set& set ) override;

private:
	compile_constant DEFAULT_PARTICLES_COUNT{ 32 };

	std::vector<Particle> particles{ DEFAULT_PARTICLES_COUNT };
	std::vector<bool>     particles_status{ DEFAULT_PARTICLES_COUNT };
};
}