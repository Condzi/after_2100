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
	sprite->set_texture( G_Resources_Storage.get_texture( "missile_player" ) );
	hitbox->s_area_entered.bond( *this,
								 [this]( Area& second ) {
									 if ( second.name != "player_hitbox" and second.name != "hitbox_missile_player" )
										 // @ToDo: fancy explosion
 										queue_for_delete();
								 } );
}

void Player_Missile::update( r32 delta )
{
	constant bounds = sprite->get_global_bounds();
	hitbox->set_rectangle_shape( bounds );

	// The missile has flown off screen
	if ( bounds.position.x > G_App.get_window_size().x ) 
		queue_for_delete();

	move( velocity * delta );
}
