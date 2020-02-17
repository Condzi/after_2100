/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"
#include "main_menu.hpp"
#include "game_master.hpp"
#include "framework/scene/application.hpp"
#include "dialogs/dialog_tutorial.hpp"

Main_Menu::Main_Menu()
{
	name = "main_menu";

	text_title = attach<Rich_Text>();
	label_new_game = attach<Clickable_Label>();
	label_exit = attach<Clickable_Label>();

	text_title->string.set_ascii( "after 2100" );
	label_new_game->text->string.set_locale_name( "#loc_new_game" );
	label_exit->text->string.set_locale_name( "#loc_exit" );

	text_title->character_size = 120;
	label_new_game->text->character_size = 70;
	label_exit->text->character_size = 70;

	text_title->update_vertices();
	label_new_game->text->update_vertices();
	label_exit->text->update_vertices();

	text_title->set_absolute_position( Percent_Position{ 20, 10 } );
	label_new_game->set_absolute_position( Percent_Position{ 40, 40 } );
	label_exit->set_absolute_position( Percent_Position{ 40, 50 } );

	bond_disconnector( label_new_game->s_label_clicked.connect( [this] {
		get_node( "root/game_master" )->cast_to<Game_Master>()->change_level<Dialog_Tutorial>();
	} ) );

	bond_disconnector( label_exit->s_label_clicked.connect( [] {
		G_App.exit_game();
	} ) );

}
