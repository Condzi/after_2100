/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/scene/application.hpp"

#include "game/flags.hpp"
#include "game_master.hpp"
#include "pause_screen.hpp"

Pause_Screen::Pause_Screen()
{
	name = "pause_screen";

	pause_title    = attach<Rich_Text>();
	label_resume   = attach<Clickable_Label>();
	label_exit     = attach<Clickable_Label>();
	options_panel  = attach<Panel>();

	pause_title->string.set_locale_name( "#loc_pause" );
	pause_title->character_size = 72;
	pause_title->update_vertices();

	label_resume->text->string.set_locale_name( "#loc_resume" );
	label_resume->text->character_size = 48;
	bond_disconnector( label_resume->s_label_clicked.connect( [] {
		G_Flags[Flags::Pause] = false;
		G_Root.get_node( "game_master" )->cast_to<Game_Master>()->update_pause_mode();
	} ) );

	label_exit->text->string.set_locale_name( "#loc_exit" );
	label_exit->text->character_size = 48;
	bond_disconnector( label_exit->s_label_clicked.connect( [] {
		G_App.exit_game();
	} ) );

	pause_title->set_absolute_position( Percent_Position{ 2,5 } );
	label_resume->set_absolute_position( Percent_Position{ 1, 20 } );
	label_exit->set_absolute_position( Percent_Position{ 1, 30 } );

	options_panel->set_size( G_Window.get_size() * 0.7 );
	options_panel->set_absolute_position( Percent_Position{ 22, 15 } );
	options_panel->set_color( { 100, 100, 100 } );
}
