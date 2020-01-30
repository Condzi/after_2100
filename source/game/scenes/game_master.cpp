/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "game_master.hpp"
#include "pause_screen.hpp"
#include "failure_screen.hpp"

#include "game/flags.hpp"

#include "game/prefabs/health.hpp"

Game_Master::Game_Master()
{
	name = "game_master";

	pause_screen = attach<Pause_Screen>();
	failure_screen = attach<Failure_Screen>();

	pause_screen->set_pause( true );
	failure_screen->set_pause( true );
}

void Game_Master::input( sf::Event const& ev )
{
	if ( ev.type is sf::Event::KeyReleased and
		 ev.key.code is sf::Keyboard::Escape and
		 failure_screen->is_paused() is true ) { // don't show pause menu when failure screen is on
		G_Flags.flip( Flags::Pause );

		update_pause_mode();
	}
}

Node* Game_Master::get_level()
{
	return level;
}

void Game_Master::reset_level()
{
	log_info( "Reseting the level ({})", level->name );

	level->queue_for_delete();
	level_instantiating_function();
	failure_screen->set_pause( true );
	// @ToDo:
	// succes_screen->set_pasue(true);
}

void Game_Master::update_pause_mode()
{
	static constant pause = G_Flags[Flags::Pause];

	pause_screen->set_pause( !pause );
	level->set_pause( pause );
	get_node( "root/game_camera" )->set_pause( pause );
}

void Game_Master::update( r32 dt )
{
	static auto level_failure = G_Flags[Flags::Level_Failure];

	unused( dt );

	// @ToDo
	if ( level_failure ) {
		failure_screen->set_pause( false );
		level->set_pause( true );
		level_failure = false;
	}
}
