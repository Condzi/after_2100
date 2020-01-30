/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "failure_screen.hpp"
#include "game_master.hpp"
#include "framework/scene/application.hpp"

Failure_Screen::Failure_Screen()
{
	name = "failure_screen";

	title_label     = attach<Rich_Text>();
	try_again_label = attach<Clickable_Label>();
	exit_label      = attach<Clickable_Label>();

	title_label->character_size = 140;
	title_label->fill_color = sf::Color{ 210, 25, 50 };
	title_label->string.set_locale_name( "#loc_level_failed" );
	title_label->update_vertices();

	try_again_label->text->character_size = 90;
	try_again_label->text->string.set_locale_name( "#loc_try_again" );

	exit_label->text->character_size = 72;
	exit_label->text->string.set_locale_name( "#loc_exit" );

	title_label->set_absolute_position( Percent_Position{ 10, 0 } );
	try_again_label->set_absolute_position( Percent_Position{ 40, 20 } );
	exit_label->set_absolute_position( Percent_Position{ 40, 35 } );

	try_again_label->s_label_clicked.connect( [this] {
		get_node( "root/game_master" )->cast_to<Game_Master>()->reset_level();
	} );
	exit_label->s_label_clicked.connect( [] {
		G_App.exit_game();
	} );
}
