/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/resources_storage.hpp"

#include "enemy_base.hpp"

Enemy_Base::Enemy_Base()
{
	sprite = attach( Sprite::instantiate() )->cast_to<Sprite>();
	sprite->name = "sprite";
	sprite->set_texture( G_Resources_Storage.get_texture( "enemy_1" ) );
	sprite->set_transformation_origin( sprite->get_global_bounds().size * 0.5 );
	sprite->rotate( -90.0deg );

	hitbox = sprite->attach( Area::instantiate() )->cast_to<Area>();
	hitbox->draw_shape = true;
	hitbox->shape_color = sf::Color::Cyan;

	health = attach( Health::instantiate() )->cast_to<Health>();

	s_on_finish_following.bond( *this,
								[&]() {
									queue_for_delete();
								} );
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
