/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "dialog_tutorial.hpp"
#include "game/scenes/game_master.hpp"
#include "game/scenes/levels/level_1.hpp"

Dialog_Tutorial::Dialog_Tutorial()
{
	name = "dialog_tutorial";

	bond_disconnector( s_dialog_finished.connect( [this] {
		get_node( "root/game_master" )->cast_to<Game_Master>()->change_level<Level_1>();
	} ) );

	set_dialog( "tutorial" );
	start();
}
