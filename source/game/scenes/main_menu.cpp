/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"
#include "main_menu.hpp"
#include "game_master.hpp"
#include "framework/scene/application.hpp"
#include "dialogs/dialog_tutorial.hpp"

#include "game/prefabs/explosion.hpp"

Main_Menu::Main_Menu()
{
	name = "main_menu";

	text_title               = attach<Rich_Text>();
	text_language            = attach<Rich_Text>();
	label_new_game           = attach<Clickable_Label>();
	label_exit               = attach<Clickable_Label>();
	label_language_selection = attach<Clickable_Label>();

	text_title->string.set_ascii( "after 2100" );
	text_language->string.set_locale_name( "#loc_language" );
	label_new_game->text->string.set_locale_name( "#loc_new_game" );
	label_exit->text->string.set_locale_name( "#loc_exit" );
	label_language_selection->text->string.set_locale_name( "#loc_en" );

	if ( G_Locale.get_current_language() is "pl" )
		label_language_selection->text->string.set_locale_name( "#loc_pl" );


	text_title->character_size = 120;
	text_language->character_size = 32;
	label_new_game->text->character_size = 70;
	label_exit->text->character_size = 70;
	label_language_selection->text->character_size = 32;

	text_title->update_vertices();
	text_language->update_vertices();
	label_new_game->text->update_vertices();
	label_exit->text->update_vertices();
	label_language_selection->text->update_vertices();

	text_title->set_absolute_position( Percent_Position{ 30, 10 } );
	text_language->set_absolute_position( Percent_Position{ 85, 90 } );
	label_new_game->set_absolute_position( Percent_Position{ 40, 40 } );
	label_exit->set_absolute_position( Percent_Position{ 40, 50 } );
	label_language_selection->set_absolute_position( Percent_Position{ 85, 95 } );

	bond_disconnector( label_new_game->s_label_clicked.connect( [this] {
		get_node( "root/game_master" )->cast_to<Game_Master>()->change_level<Dialog_Tutorial>();
	} ) );

	bond_disconnector( label_exit->s_label_clicked.connect( [] {
		G_App.exit_game();
	} ) );

	bond_disconnector( label_language_selection->s_label_clicked.connect( [this] {

		if ( G_Locale.get_current_language() is "pl" ) {
			G_Locale.set_current_language( "en" );
			label_language_selection->text->string.set_locale_name( "#loc_en" );
		} else {
			G_Locale.set_current_language( "pl" );
			label_language_selection->text->string.set_locale_name( "#loc_pl" );
		}
	} ) );


	auto explosion = attach<Explosion>();

	explosion->set_global_position( { 640, 200 } );
	explosion->layer = 1;
	explosion->set_scale( { 2,2 } );
	explosion->setup();
	explosion->explode();
}
