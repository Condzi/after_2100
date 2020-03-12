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
		struct Color
		{
			Color( u8 r_ = 255, u8 g_ = 255, u8 b_ = 255, u8 a_ = 255 )
			{
				r = static_cast<r32>( r_ );
				g = static_cast<r32>( g_ );
				b = static_cast<r32>( b_ );
				a = static_cast<r32>( a_ );
			}

			r32 r, g, b, a;
		} color;
	};

public:
	std::function<void( Particle&, r32 )> custom_transformation; // Ref to particle and dt

	bool is_emmiting{ false };
	s32 layer;

	struct
	{
		sf::Texture const* texture{ nullptr };
		Rectangle_Shape    texture_rect;
		r32                scale{ 0.1f };
		sf::Color          color{ sf::Color::White };

		r32 lifetime{ 1.0sec };
		r32 spawn_interval{ 0.1sec };
		s32 particles_limit{ 16 }; // Don't declare greater than DEFAULT_PARTICLES_COUNT

		r32  spin_velocity{ 0 }; // in degrees
		Vec2 initial_velocity{ 0,0 };
		r32  random_initial_velocity_min{ 100.0f };
		r32  random_initial_velocity_max{ 100.0f };
		Vec2 constant_force_to_apply{ 0,0 }; // Gravity etc. Applied every frame. @ToDo: Add separate apply_force function for impulses?
		Vec2 impulse_force_to_apply{ 0,0 }; // zeroed every frame
		r32	 angle_min{ 0 }; // Minimum angle in which particles are emitted, in degress.
		r32  angle_max{ 360 }; // Maximum angle in which particles are emitted, in degress.

		bool one_shot{ false }; // release all particles once.
	} settings;

	Particle_Emitter();

	void set_particles_count( s32 count );

	[[nodiscard]] auto get_particles_count() const -> s32;

	void update( r32 dt ) override;
	void draw( Drawing_Set& set ) override;

	void apply_force( Vec2 const& force );

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