/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "game_master.hpp"
#include "pause_screen.hpp"

#include "game/flags.hpp"

#include "game/prefabs/health.hpp"

Game_Master::Game_Master()
{
	name = "game_master";

	pause_screen = attach<Pause_Screen>();
	pause_screen->set_pause( true );
}

void Game_Master::input( sf::Event const& ev )
{
	if ( ev.type is sf::Event::KeyReleased and
		 ev.key.code is sf::Keyboard::Escape ) {
		G_Flags.flip( Flags::Pause );

		update_pause_mode();
	}
}

Node* Game_Master::get_level()
{
	return level;
}

void Game_Master::update_pause_mode()
{
	pause_screen->set_pause( !G_Flags[Flags::Pause] );
	level->set_pause( G_Flags[Flags::Pause] );
	get_node( "root/game_camera" )->set_pause( G_Flags[Flags::Pause] );
}

void Game_Master::update( r32 dt )
{
	unused( dt );

	// @ToDo
	if ( level->get_node( "player/health" )->cast_to<Health>()->is_dead() )
		log_info( "Player is dead!" );
}
