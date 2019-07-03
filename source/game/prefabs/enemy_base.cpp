/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/resources_storage.hpp"

#include "enemy_base.hpp"

Enemy_Base::Enemy_Base()
{
	name = "enemy_base";

	sprite = attach( Sprite::instantiate() )->cast_to<Sprite>();
	sprite->name = "sprite";
	sprite->set_texture( G_Resources_Storage.get_texture( "enemy_1" ) );
	sprite->set_transformation_origin( sprite->get_global_bounds().size * 0.5 );
	sprite->rotate( -90.0deg );

	hitbox = sprite->attach( Area::instantiate() )->cast_to<Area>();
	hitbox->shape_color = sf::Color::Cyan;
	hitbox->name = "enemy_base_hitbox";

	bond_disconnector( hitbox->s_area_entered.connect(
		[this]( Area& second ) {
			if ( second.name == "hitbox_missile_player" )
				health->damage( 1 );
		} ) );

	health = attach( Health::instantiate() )->cast_to<Health>();
	health->set_max( 2 );
	bond_disconnector( health->s_dead.connect( [this] { queue_for_delete(); } ) );

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
