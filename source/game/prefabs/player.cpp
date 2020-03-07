/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/audio_listener.hpp"
#include "framework/common/resources_storage.hpp"
#include "framework/scene/application.hpp"
#include "framework/common/shapes.hpp"

#include "player.hpp"
#include "game/flags.hpp"

#include <SFML/Window/Keyboard.hpp>

Player::Player()
{
	name = "player";

	sprite = attach<Sprite>();
	sprite->name = "sprite";
	sprite->set_texture_from_name( "player" );
	sprite->layer = 3;

	constant sprite_size = sprite->get_global_bounds().size;
	// After rotating by 90deg width gets swapped with height.
	sprite->set_local_position( { sprite_size.height / 2, sprite_size.width / 2 } );
	sprite->set_transformation_origin( sprite_size * 0.5 );

	hitbox = sprite->attach<Area>();
	hitbox->name = "hitbox_" + name;
	hitbox->set_circle_shape( { sprite->get_global_bounds().position + sprite_size * 0.5, sprite->get_global_bounds().size.width / 2 } );

	bond_disconnector( s_move.connect( [&]( auto offset ) {
		std::get<Circle_Shape>( hitbox->get_shape_variant() ).center += offset;
	} ) );

	gun_a = sprite->attach<Missile_Shooter>();
	gun_b = sprite->attach<Missile_Shooter>();

	gun_a->name = "gun_a";
	gun_b->name = "gun_b";

	gun_a->set_missile_type<Player_Missile>();
	gun_b->set_missile_type<Player_Missile>();

	gun_a->set_horizontal_velocity( 500 );
	gun_b->set_horizontal_velocity( 500 );

	gun_a->set_local_position( Size2{ 50.0px, -36.0px } );
	gun_b->set_local_position( Size2{ 50.0px, 36.0px } );

	gun_a->set_cooldown_time( 0.2sec );
	gun_b->set_cooldown_time( 0.2sec );


	explosion = attach<Explosion>();
	explosion->layer = 5;
	explosion->setup();

	exploded_sprite = attach<Exploded_Sprite>();
	exploded_sprite->name = "exploded_sprite";
	exploded_sprite->set_texture_from_pointer( sprite->get_texture() );
	exploded_sprite->degress_per_second = random_real( -360, 360 );
	exploded_sprite->scale_per_second = 0.40;
	exploded_sprite->initialize( { 120, 180 } );
	exploded_sprite->visible = false;
	exploded_sprite->set_global_position( sprite->get_sprite_raw().getPosition()- cast<sf::Vector2f>( sprite->get_global_bounds().size * 0.5 ) );
	exploded_sprite->layer = 3;

	health = attach<Health>();
	health->set_max( 2 );

	low_health_sound = attach<Sound_Source>();
	low_health_sound->name = "low_health";
	low_health_sound->set_sound_buffer_from_name( "low_hp" );
	low_health_sound->set_relative_to_audio_listener( false );

	bond_disconnector( health->s_dead.connect( [this] {
		sprite->visible = false;
		hitbox->collision_layer = -1;
		hitbox->shape_color.a -= 200;
		explosion->explode();
		exploded_sprite->visible = true;
		exploded_sprite->explode();
		get_node( "root/game_camera" )->cast_to<Camera>()->add_shake_trauma( 0.25f );
	} ) );

	bond_disconnector( exploded_sprite->s_done_scaling.connect( [] {
		G_Flags[Flags::Level_Failure] = true;
	} ) );

	low_hp_particles = attach<Particle_Emitter>();
	{
		auto& settings = low_hp_particles->settings;
		settings.angle_min = 0.0deg;
		settings.angle_max = 360.0deg;
		settings.color = sf::Color{ 240,190,15, 150 };
		settings.texture = G_Resources_Storage.get_texture( "round_particle" );
		settings.initial_velocity_min = 256;
		settings.initial_velocity_max = 300;
		settings.lifetime = 0.5sec;
		settings.spawn_interval = 0.005sec;
		settings.particles_limit = 160;
		settings.scale = 0.12;
		low_hp_particles->set_particles_count( 160 );
	}

	low_hp_smoke = attach<Particle_Emitter>();
	{
		auto& settings = low_hp_smoke->settings;
		settings.angle_min = 240.0deg;
		settings.angle_max = 300.0deg;
		settings.color = sf::Color{ 90,90,90, 200 };
		settings.texture = G_Resources_Storage.get_texture( "round_particle" );
		settings.initial_velocity_min = 160;
		settings.initial_velocity_max = 190;
		settings.lifetime = 3.0sec;
		settings.spawn_interval = 0.01sec;
		settings.particles_limit = 512;
		settings.scale = 0.4;
		low_hp_smoke->set_particles_count( 512 );
	}

	low_hp_particles->layer = 2;
	low_hp_smoke->layer = 4;

	low_hp_particles->custom_transformation = [ptr = low_hp_particles]( auto& particle, r32 ) {
		particle.color.a = ( particle.remaining_lifetime / ptr->settings.lifetime ) * ptr->settings.color.a;
	};

	low_hp_smoke->custom_transformation = [ptr = low_hp_smoke]( auto& particle, r32 ) {
		particle.color.a = ( particle.remaining_lifetime / ptr->settings.lifetime ) * ptr->settings.color.a;
	};

	low_hp_particles->set_local_position( { sprite_size.height/ 2, sprite_size.width / 2 } );
	low_hp_smoke->set_local_position( { sprite_size.height/ 2, sprite_size.width / 2 } );

	set_absolute_position( Percent_Position{ 0, 45 } );

	rotate( 90.0deg );
}

void Player::update( r32 dt )
{
	if ( health->is_dead() ) {
		low_hp_particles->is_emmiting = false;
		low_hp_smoke->is_emmiting = false;
		return;
	}

	if ( health->get_current() is 1 ) {
		low_hp_particles->is_emmiting = true;
		low_hp_smoke->is_emmiting = true;

		if ( not low_health_sound->is_playing() ) {
			low_health_sound->play();
			low_health_sound->set_loop( true );
		}
	} else {
		low_health_sound->set_loop( false );
		low_hp_particles->is_emmiting = false;
		low_hp_smoke->is_emmiting = false;
	}

	G_Audio_Listener.set_position( get_global_position() );

	check_movement_keys();
	move( velocity * dt );
	correct_for_boundary_collision();
	slow_down();
	accelerate( dt );
	update_tilt_transformation();
}

void Player::check_movement_keys()
{
	G_Profile_Function();

	if ( G_Window.is_focused() is false )
		return;

	acceleration_direction = Vec2::Zero();

	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) and velocity.x > -VELOCITY_MAX )
		acceleration_direction += Vec2::Left();
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) and velocity.x < VELOCITY_MAX )
		acceleration_direction += Vec2::Right();
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) and velocity.y > -VELOCITY_MAX )
		acceleration_direction += Vec2::Up();
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) and velocity.y < VELOCITY_MAX )
		acceleration_direction += Vec2::Down();

	// Should be in own method?
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) and
		 gun_a->is_ready_to_shoot() ) {

		gun_a->shoot();
		gun_b->shoot();

		// @ToDo: Separate variable for recoil? According to Newton's law (am = -am)
		// we need some mass multiplier.
		constant recoil = gun_a->get_horizontal_velocity() * 0.8;
		velocity.x -= recoil;
	}
}

void Player::slow_down()
{
	G_Profile_Function();

	if ( acceleration_direction.x is 0 ) {
		velocity.x *= SLOWING_MULTIPLIER;
		if ( std::fabs( velocity.x ) < 1 )
			velocity.x = 0;
	}
	if ( acceleration_direction.y is 0 ) {
		velocity.y *= SLOWING_MULTIPLIER;
		if ( std::fabs( velocity.y ) < 1 )
			velocity.y = 0;
	}
}

void Player::accelerate( r32 dt )
{
	velocity += acceleration_direction * VELOCITY_MAX * ACCELERATION_MULTIPLIER;
}

void Player::correct_for_boundary_collision()
{
	G_Profile_Function();

	constant window_size = G_Window.get_size();
	constant sprite_size = sprite->get_global_bounds().size;

	constant x_pos     = get_global_position().x;
	constant x_pos_max = x_pos + sprite_size.width;
	constant y_pos     = get_global_position().y;
	constant y_pos_max = y_pos + sprite_size.height;

	auto new_position = get_global_position();

	if ( x_pos < 0 ) {
		new_position.x = 0;

		if ( acceleration_direction.x is -1 )
			acceleration_direction.x = 0;
	} else if ( x_pos_max > window_size.width ) {
		new_position.x = window_size.width - sprite_size.width;

		if ( acceleration_direction.x is 1 )
			acceleration_direction.x = 0;
	}

	if ( y_pos < 0 ) {
		new_position.y = 0;
		if ( acceleration_direction.y is -1 )
			acceleration_direction.y = 0;
	} else if ( y_pos_max > window_size.height ) {
		new_position.y = window_size.height - sprite_size.height;

		if ( acceleration_direction.y is 1 )
			acceleration_direction.y = 0;
	}

	set_global_position( new_position );
}

void Player::update_tilt_transformation()
{
	G_Profile_Function();

	// Doesn't matter from whitch sprite we are getting the values.
	auto [pitch, yaw, roll] = sprite->get_rotation_3d();

	pitch = velocity.x * TILT_MULTIPLIER;
	yaw = velocity.y * TILT_MULTIPLIER;

	sprite->set_rotation_3d( pitch, yaw, roll );
}