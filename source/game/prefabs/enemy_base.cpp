/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "enemy_base.hpp"

#include "game/scenes/game_master.hpp"

#include "framework/common/resources_storage.hpp"
#include "framework/scene/camera.hpp"

Enemy_Base::Enemy_Base()
{
	name = "enemy_base";

	sprite = attach<Sprite>();
	sprite->name = "sprite";
	sprite->set_texture_from_name( "enemy_1" );
	sprite->set_transformation_origin( sprite->get_global_bounds().size * 0.5 );
	sprite->rotate( -90.0deg );
	sprite->layer = 3;

	hitbox = sprite->attach<Area>();
	hitbox->shape_color = sf::Color::Cyan;
	hitbox->name = "hitbox_" + name;

	exploded_sprite = attach<Exploded_Sprite>();
	exploded_sprite->name = "exploded_sprite";
	exploded_sprite->set_texture_from_pointer( sprite->get_texture() );
	exploded_sprite->rotate( -90.0deg );
	exploded_sprite->degress_per_second = random_real( -360, 360 );
	exploded_sprite->initialize( { 120, 180 } );
	exploded_sprite->visible = false;
	// hack - positioning sprite (and animation) right in the center of the real sprite
	exploded_sprite->set_global_position( sprite->get_sprite_raw().getPosition()- cast<sf::Vector2f>( sprite->get_global_bounds().size * 0.5 ) );
	exploded_sprite->layer = 1;

	health = attach<Health>();
	health->set_max( 2 );

	explosion = attach<Explosion>();
	explosion->sprite->layer = 4;
	explosion->set_scale( { 2.0, 2.0 } );
	explosion->sprite->set_transformation_origin( explosion->get_frame_size() * 0.5f );

	bond_disconnector( hitbox->s_area_entered.connect(
		[this]( Area& second ) {
		auto player_node = get_node( "root/game_master" )->cast_to<Game_Master>()->get_level()->get_node( "player" );
		auto player_health = player_node->get_node( "health" )->cast_to<Health>();

		if ( second.name is "hitbox_missile_player" )
			health->damage( 1, player_node );
		else if ( second.name is "hitbox_player" ) {
			health->kill( player_node );
			player_health->damage( 1, this );
		}
	} ) );

	bond_disconnector( health->s_dead.connect( [this] {
		sprite->visible = false;
		hitbox->collision_layer = -1;
		hitbox->shape_color.a -= 200;
		stop_following();
		explosion->play();
		exploded_sprite->visible = true;
		exploded_sprite->explode();
		get_node( "root/game_camera" )->cast_to<Camera>()->add_shake_trauma( 0.25f );
	} ) );

	bond_disconnector( exploded_sprite->s_done_scaling.connect( [this] { queue_for_delete(); } ) );

	bond_disconnector( s_on_finish_following.connect(
		[&] {
		queue_for_delete();
	} ) );
}

void Enemy_Base::update( r32 dt )
{
	Path_Follower::update( dt );

	update_tilt_transformation();
}

void Enemy_Base::update_tilt_transformation()
{
	auto [pitch, yaw, roll] = sprite->get_rotation_3d();

	pitch = get_velocity().x * TILT_MULTIPLIER;
	yaw = get_velocity().y * TILT_MULTIPLIER;

	sprite->set_rotation_3d( pitch, yaw, roll );

	hitbox->set_rectangle_shape( sprite->get_global_bounds() );
}