/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/
#pragma once

#include "node.hpp"

#include <SFML/Graphics/Sprite.hpp>

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
		Vec2 velocity;
		r32  rotation;
		r32  remaining_lifetime;
		sf::Color color;
	};

public:
	std::function<void( Particle& )> custom_transformation;

	s32 layer;

	struct
	{
		sf::Texture const* texture{ nullptr };
		Rectangle_Shape    texture_rect;
		sf::Color          color{ sf::Color::White };

		r32 lifetime{ 10.0sec };
		r32 spawn_interval{ 0.1sec };
		s32 particles_limit{ 16 }; // Don't declare greater than DEFAULT_PARTICLES_COUNT

		r32  spin_velocity{ 0 }; // in degrees
		r32  initial_velocity{ 100.0f };
		Vec2 force_to_apply; // Gravity etc. Applied every frame. @ToDo: Add separate apply_force function for impulses?
		r32	 angle_min{ 0 }; // Minimum angle in which particles are emitted, in degress.
		r32  angle_max{ 180.0deg }; // Maximum angle in which particles are emitted, in degress.

		bool one_shot{ false }; // release all particles once.
	} settings;

	Particle_Emitter();

	void set_particles_count( s32 count );

	[[nodiscard]] auto get_particles_count() const -> s32;

	void update( r32 dt ) override;
	void draw( Drawing_Set& set ) override;

private:
	// Note: arrays share indexes.
	compile_constant      DEFAULT_PARTICLES_COUNT{ 32 };
	s32                   particles_count{ DEFAULT_PARTICLES_COUNT };
	std::vector<Particle> particles{ DEFAULT_PARTICLES_COUNT };
	std::vector<bool>     particles_status;

	// @Performance: use sf::VertexArray instead.
	std::vector<sf::Sprite> particles_sprites{ DEFAULT_PARTICLES_COUNT };

	r32 time_to_next_spawn{ settings.spawn_interval };
	s32 released_particles_count{ 0 }; // For settings.one_shot.

	void spawn_particle();
};
}