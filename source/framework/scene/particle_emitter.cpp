/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/stl_extensions.hpp"

#include "particle_emitter.hpp"

namespace con
{
Particle_Emitter::Particle_Emitter()
{
	name = "Particle_Emitter";

	// Has to be here because yes.
	particles_status.resize( particles_count );
}

void Particle_Emitter::set_particles_count( s32 count )
{
	particles_count = count;

	particles.resize( count );
	particles_status.resize( count );
	particles_sprites.resize( count );
}

auto Particle_Emitter::get_particles_count() const -> s32
{
	return particles_count;
}

void Particle_Emitter::update( r32 dt )
{
	if ( is_emmiting ) {
		time_to_next_spawn -= dt;

		if ( ( settings.one_shot and released_particles_count < settings.particles_limit ) or
			 !settings.one_shot )
			while ( time_to_next_spawn < 0 ) {
				time_to_next_spawn += settings.spawn_interval;

				spawn_particle();
			}
	}

	for ( size_t i = 0; i < particles_count; i++ ) {
		if ( particles_status[i] is false ) continue;

		auto& particle = particles[i];

		particle.remaining_lifetime -= dt;

		if ( particle.remaining_lifetime <= 0 ) {
			particles_status[i] = false;
			continue;
		}

		particle.velocity += settings.force_to_apply;
		particle.position += particle.velocity * dt;
		particle.rotation += settings.spin_velocity * dt;

		if ( custom_transformation )
			custom_transformation( particle, dt );
	}
}

void Particle_Emitter::draw( Drawing_Set& set )
{
	for ( size_t i = 0; i < particles_count; i++ ) {
		if ( particles_status[i] is false ) continue;

		auto& particle = particles[i];
		auto& spr = particles_sprites[i];

		spr.setPosition( particle.position );
		spr.setColor( particle.color );
		spr.setRotation( particle.rotation );

		set.add_drawable( spr, layer );
	}
}

void Particle_Emitter::spawn_particle()
{
	if ( settings.texture is nullptr ) {
		engine_log_warning( "Particle's texture is nullptr." );
		return;
	}

	auto const [found, idx] = find( particles_status, false );

	if ( not found ) {
		engine_log_warning( "Couldn't spawn the particle, there's no more space ( the limit is {} ).", particles_count );
		return;
	}

	particles_status[idx] = true;
	auto& particle = particles[idx];

	particle.position	        = get_global_position();
	constant angle_to_set       = random_real( settings.angle_min, settings.angle_max ) * 3.1415 / 180;
	constant velocity_to_set    = random_real( settings.initial_velocity_min, settings.initial_velocity_max );
	particle.velocity	        = Vec2{ std::sinf( angle_to_set ), std::cosf( angle_to_set ) } * velocity_to_set;
	particle.remaining_lifetime = settings.lifetime;
	particle.color              = settings.color;

	auto& spr = particles_sprites[idx];

	spr.setPosition( particle.position );
	spr.setTexture( *settings.texture );
	spr.setScale( settings.scale, settings.scale );
	// @Hack: Check if texture_rect is set.
	if ( settings.texture_rect.size.width * settings.texture_rect.size.height > 0 )
		spr.setTextureRect( settings.texture_rect );

	// Set bounds in center.
	spr.setOrigin( spr.getGlobalBounds().width / 2, spr.getGlobalBounds().height / 2 );

	released_particles_count++;
}
}