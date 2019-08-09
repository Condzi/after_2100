/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/resources_storage.hpp"
#include "enemy_base.hpp"
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

	exploded_sprite = attach<Exploded_Sprite>();
	exploded_sprite->name = "exploded_sprite";
	exploded_sprite->set_texture_from_pointer( sprite->get_texture() );
	exploded_sprite->rotate( -90.0deg );
	exploded_sprite->degress_per_second = random_real( -360, 360 );
	exploded_sprite->initialize( { 120, 180 } );
	exploded_sprite->visible = false;
	exploded_sprite->set_pause( true );
	// hack - positioning sprite (and animation) right in the center of the real sprite
	exploded_sprite->set_global_position( sprite->get_sprite_raw().getPosition()- static_cast<sf::Vector2f>( sprite->get_global_bounds().size * 0.5 ) );
	exploded_sprite->layer = 1;

	hitbox = sprite->attach<Area>();
	hitbox->shape_color = sf::Color::Cyan;
	hitbox->name = "hitbox_" + name;

	health = attach<Health>();
	health->set_max( 2 );

	explosion = attach<Explosion>();
	explosion->sprite->layer = 4;
	// hack (see exploded_sprite->set_global position above)
	explosion->set_global_position( static_cast<Vec2>( sprite->get_sprite_raw().getPosition() ) - sprite->get_global_bounds().size  );
	// slightly moving it to the left
	explosion->move( {explosion->get_frame_size().width * -0.25f, 0} );

	explosion->set_scale( { 2.0, 2.0 } );

	explosion->sprite->layer = 4;
	exploded_sprite->layer = 3;

	bond_disconnector( hitbox->s_area_entered.connect(
		[this]( Area& second ) {
			if ( second.name == "hitbox_missile_player" )
				health->damage( 1 );
		} ) );

	bond_disconnector( health->s_dead.connect( [this] {
		sprite->visible = false;
		hitbox->set_pause( true );
		stop_following();
		explosion->play();
		exploded_sprite->visible = true;
		exploded_sprite->set_pause( false );
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
