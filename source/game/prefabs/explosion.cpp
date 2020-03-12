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
	particles_d = attach<Particle_Emitter>();
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

	constant texture = G_Resources_Storage.get_texture( "explosion_particle" );;
	auto& settings_a = particles_a->settings;
	auto& settings_b = particles_b->settings;
	auto& settings_c = particles_c->settings;
	auto& settings_d = particles_d->settings;

	particles_a->set_particles_count( 16 );
	settings_a.color ={ 255, 245, 190 };
	settings_a.one_shot = true;
	settings_a.scale = 0.4;
	settings_a.particles_limit = 16;
	settings_a.lifetime = 0.5sec;
	settings_a.spawn_interval = 0.0001sec;
	settings_a.texture = texture;
	settings_a.random_initial_velocity_min = 25;
	settings_a.random_initial_velocity_max = 65;
	settings_a.angle_min = 0.0deg;
	settings_a.angle_max = 360.0deg;
	settings_a.random_spin_velocity_min = -220.0deg;
	settings_a.random_spin_velocity_max = 220.0deg;
	particles_a->custom_transformation = [ptr = particles_a]( auto& particle, r32 ) {
		particle.color.a = ( particle.remaining_lifetime / ptr->settings.lifetime ) * ptr->settings.color.a;
	};
	particles_a->layer = layer;


	particles_b->set_particles_count( 32 );
	settings_b.color ={ 255, 215, 128 };
	settings_b.one_shot = true;
	settings_b.scale = 0.5;
	settings_b.particles_limit = 32;
	settings_b.lifetime = 0.5sec;
	settings_b.spawn_interval = 0.0001sec;
	settings_b.texture = texture;
	settings_b.random_initial_velocity_min = 75;
	settings_b.random_initial_velocity_max = 125;
	settings_b.angle_min = 0.0deg;
	settings_b.angle_max = 360.0deg;
	settings_b.random_spin_velocity_min = -180.0deg;
	settings_b.random_spin_velocity_max = 180.0deg;
	particles_b->custom_transformation = [ptr = particles_b]( auto& particle, r32 ) {
		particle.color.a = ( particle.remaining_lifetime / ptr->settings.lifetime ) * ptr->settings.color.a;
	};
	particles_b->layer = layer;

	particles_c->set_particles_count( 64 );
	settings_c.color ={ 255, 170, 0 };
	settings_c.one_shot = true;
	settings_c.scale = 0.6;
	settings_c.particles_limit = 64;
	settings_c.lifetime = 0.6sec;
	settings_c.spawn_interval = 0.0001sec;
	settings_c.texture = texture;
	settings_c.random_initial_velocity_min = 150;
	settings_c.random_initial_velocity_max = 200;
	settings_c.angle_min = 0.0deg;
	settings_c.angle_max = 360.0deg;
	settings_c.random_spin_velocity_min = -120.0deg;
	settings_c.random_spin_velocity_max = 120.0deg;
	particles_c->custom_transformation = [ptr = particles_c]( auto& particle, r32 ) {
		particle.color.a = ( particle.remaining_lifetime / ptr->settings.lifetime ) * ptr->settings.color.a;
	};
	particles_c->layer = layer;

	particles_d->set_particles_count( 64 );
	settings_d.color ={ 70, 70,70 };
	settings_d.one_shot = true;
	settings_d.scale = 0.8;
	settings_d.particles_limit = 64;
	settings_d.lifetime = 0.8sec;
	settings_d.spawn_interval = 0.001sec;
	settings_d.texture = G_Resources_Storage.get_texture( "smoke_particle" );
	settings_d.random_initial_velocity_min = 150;
	settings_d.random_initial_velocity_max = 200;
	settings_d.angle_min = 0.0deg;
	settings_d.angle_max = 360.0deg;
	particles_d->custom_transformation = [ptr = particles_d]( auto& particle, r32 ) {
		particle.color.a = ( particle.remaining_lifetime / ptr->settings.lifetime ) * ptr->settings.color.a;
	};
	particles_d->layer = layer;
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
	particles_d->is_emmiting = true;

	sound->play();
}
