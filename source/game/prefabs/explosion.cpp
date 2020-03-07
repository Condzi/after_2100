/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "explosion.hpp"

#include "framework/scene/sound_source.hpp"

Explosion::Explosion()
{
	name = "explosion";
	
	// Initialize in this order to proper layering (brighter particles on top, then dimmer, then smoke at the bottom)
	particles_c = attach<Particle_Emitter>();
	particles_b = attach<Particle_Emitter>();
	particles_a = attach<Particle_Emitter>();

	sound = attach<Sound_Source>();
	sound->name = "explosion_sound";
	sound->set_sound_buffer_from_name( "explosion" );
	// Setting it to 0 makes better results that to 1, because it fades out too quickly.
	sound->set_attenuation( 0 );
}

void Explosion::setup()
{
	is_ready = true;

	particles_a->set_particles_count( 128 );
	particles_a->settings.color = { 255, 215, 128 };
	particles_a->settings.one_shot = true;
	particles_a->settings.scale = 0.3;
	particles_a->settings.particles_limit = 128;
	particles_a->settings.lifetime = 0.6sec;
	particles_a->settings.spawn_interval = 0.0001sec;
	particles_a->settings.texture = G_Resources_Storage.get_texture( "round_particle" );
	particles_a->settings.initial_velocity_min = 50;
	particles_a->settings.initial_velocity_max = 100;
	particles_a->settings.angle_min = 0.0deg;
	particles_a->settings.angle_max = 360.0deg;
	particles_a->custom_transformation = [ptr = particles_a]( auto& particle, r32 ) {
		particle.color.a = ( particle.remaining_lifetime / ptr->settings.lifetime ) * ptr->settings.color.a;
	};
	particles_a->layer = layer;

	particles_b->set_particles_count( 64 );
	particles_b->settings.color = { 255, 170, 0 };
	particles_b->settings.one_shot = true;
	particles_b->settings.scale = 0.6;
	particles_b->settings.particles_limit = 64;
	particles_b->settings.lifetime = 0.8sec;
	particles_b->settings.spawn_interval = 0.0001sec;
	particles_b->settings.texture = G_Resources_Storage.get_texture( "round_particle" );
	particles_b->settings.initial_velocity_min = 100;
	particles_b->settings.initial_velocity_max = 150;
	particles_b->settings.angle_min = 0.0deg;
	particles_b->settings.angle_max = 360.0deg;
	particles_b->custom_transformation = [ptr = particles_b]( auto& particle, r32 ) {
		particle.color.a = ( particle.remaining_lifetime / ptr->settings.lifetime ) * ptr->settings.color.a;
	};
	particles_b->layer = layer;

	particles_c->set_particles_count( 32 );
	particles_c->settings.color = { 70, 70,70 };
	particles_c->settings.one_shot = true;
	particles_c->settings.scale = 0.8;
	particles_c->settings.particles_limit = 32;
	particles_c->settings.lifetime = 3.0sec;
	particles_c->settings.spawn_interval = 0.0001sec;
	particles_c->settings.texture = G_Resources_Storage.get_texture( "round_particle" );
	particles_c->settings.initial_velocity_min = 50;
	particles_c->settings.initial_velocity_max = 100;
	particles_c->settings.angle_min = 0.0deg;
	particles_c->settings.angle_max = 360.0deg;
	particles_c->custom_transformation = [ptr = particles_c]( auto& particle, r32 ) {
		particle.color.a = ( particle.remaining_lifetime / ptr->settings.lifetime ) * ptr->settings.color.a;
	};
	particles_c->layer = layer;
}

void Explosion::explode()
{
	if ( not is_ready ) {
		log_warning( "Can't emit explosion because it hasn't been set up." );
		return;
	}

	particles_a->is_emmiting = true;
	particles_b->is_emmiting = true;
	particles_c->is_emmiting = true;

	sound->play();
}
