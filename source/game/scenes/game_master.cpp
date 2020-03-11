/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/stl_extensions.hpp"

#include "game_master.hpp"
#include "pause_screen.hpp"
#include "failure_screen.hpp"
#include "success_screen.hpp"

#include "game/flags.hpp"

#include "game/prefabs/health.hpp"

Game_Master::Game_Master()
{
	name = "game_master";

	pause_screen = attach<Pause_Screen>();
	failure_screen = attach<Failure_Screen>();
	success_screen = attach<Success_Screen>();

	pause_screen->set_pause( true );
	failure_screen->set_pause( true );
	success_screen->set_pause( true );
}

void Game_Master::input( sf::Event const& ev )
{
	if ( ev.type is sf::Event::KeyReleased and
		 ev.key.code is sf::Keyboard::Escape and
		 failure_screen->is_paused() and success_screen->is_paused() and // don't show pause menu when failure or success screen is on
		 string_begins_with( level->name, "level" ) ) { // only usable in actual levels @ToDo: add flag when switching levels that is set once, don't check this condition every time here.
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

	level_instantiating_function();
	failure_screen->set_pause( true );
	success_screen->set_pause( true );
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
	unused( dt );

	static auto level_failure = G_Flags[Flags::Level_Failure];
	static auto level_success = G_Flags[Flags::Level_Success];

	// @ToDo: Special screen: ,,We've won... but at what cost?'' with waluigi!
	if ( level_failure or ( level_failure and level_success ) ) {
		failure_screen->set_pause( false );
		level->set_pause( true );
		level_failure = false;
		level_success = false;
	} else if ( level_success ) {
		success_screen->set_pause( false );
		level->set_pause( true );
		level_success = false;
	}
}
