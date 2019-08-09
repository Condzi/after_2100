/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/resources_storage.hpp"
#include "framework/scene/application.hpp"
#include "player_missile.hpp"
#include "health.hpp"

Player_Missile::Player_Missile()
{
	name = "missile_player";
	hitbox->name = "hitbox_missile_player";

	missile_animation = attach<Animation>();

	missile_animation->set_texture_from_name( "missile_player" );
	missile_animation->set_is_looped( true );
	missile_animation->set_frame_size( { 178, 363 } );
	missile_animation->set_frames_count( 3 );
	missile_animation->set_duration( 0.2sec );
	missile_animation->play();
	missile_animation->set_scale( { 0.2, 0.2 } );
	missile_animation->sprite->set_transformation_origin( missile_animation->sprite->get_global_bounds().size * 0.5 );
	missile_animation->rotate( 90.0deg );
	missile_animation->sprite->layer = 2;

	bond_disconnector( hitbox->s_area_entered.connect(
		[this]( Area& second ) {
			if ( second.name != "hitbox_player" and second.name != "hitbox_missile_player" )
				// @ToDo: fancy explosion
				queue_for_delete();
		} ) );
}

void Player_Missile::update( r32 delta )
{
	constant bounds = missile_animation->sprite->get_global_bounds();
	hitbox->set_rectangle_shape( bounds );

	// The missile has flown off screen
	if ( bounds.position.x > G_Window.get_size().x )
		queue_for_delete();

	move( velocity * delta );
}
