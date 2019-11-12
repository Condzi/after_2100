/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/scene/application.hpp"

#include "game/flags.hpp"

#include "pause_screen.hpp"

Pause_Screen::Pause_Screen()
{
	pause_title   = attach<Rich_Text>();
	label_resume  = attach<Clickable_Label>();
	label_exit    = attach<Clickable_Label>();
	options_panel = attach<Panel>();

	pause_title->string.set_locale_name( "#loc_pause" );
	pause_title->character_size = 72;
	pause_title->update_vertices();

	label_resume->text->string.set_locale_name( "#loc_resume" );
	label_resume->text->character_size = 48;
	bond_disconnector( label_resume->s_label_clicked.connect( [] {
		if ( G_Flags[Flags::Pause] )
			G_Flags[Flags::Pause] = false;
					   } ) );

	label_exit->text->string.set_locale_name( "#loc_exit" );
	label_exit->text->character_size = 48;
	bond_disconnector( label_exit->s_label_clicked.connect( [] {
		if ( G_Flags[Flags::Pause] )
			G_App.exit_game();
					   } ) );

	pause_title->set_absolute_position( Percent_Position{ 2,5 } );
	label_resume->set_absolute_position( Percent_Position{ 1, 20 } );
	label_exit->set_absolute_position( Percent_Position{ 1, 30 } );

	options_panel->set_size( G_Window.get_size() * 0.7 );
	options_panel->set_absolute_position( Percent_Position{ 22, 15 } );
	options_panel->set_color( { 100, 100, 100 } );
}

void Pause_Screen::input( sf::Event const& ev )
{
	if ( ev.type is sf::Event::KeyReleased and
		 ev.key.code is sf::Keyboard::Escape )
		G_Flags.flip( Flags::Pause );
}

void Pause_Screen::update( r32 dt )
{
	unused( dt );

	pause_title->visible        = G_Flags[Flags::Pause];
	label_resume->text->visible = G_Flags[Flags::Pause];
	label_exit->text->visible   = G_Flags[Flags::Pause];
	options_panel->visible      = G_Flags[Flags::Pause];

}