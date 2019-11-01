/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "pause_screen.hpp"

#include "framework/scene/application.hpp"


Pause_Screen::Pause_Screen()
{
	pause_title = attach<Label>();

	pause_title->string.set_locale_name( "#loc_pause" );
	pause_title->set_absolute_position( Percent_Position{ 45,25 } );
	pause_title->set_character_size( 72 );

	option_resume = attach<Clickable_Label>();
	option_resume->text->string.set_locale_name( "#loc_resume" );
	option_resume->text->character_size = 48;
	bond_disconnector( option_resume->s_label_clicked.connect( [] {
		if ( G_App.is_paused() ) {
			G_App.toggle_pause();
			G_Root.set_pause( false );
		}
					   } ) );

	option_exit = attach<Clickable_Label>();
	option_exit->text->string.set_locale_name( "#loc_exit" );
	option_exit->text->character_size = 48;
	bond_disconnector( option_exit->s_label_clicked.connect( [] {
		if ( G_App.is_paused() )
			G_App.exit_game();
					   } ) );

	option_resume->set_absolute_position( Percent_Position{ 45, 45 } );
	option_exit->set_absolute_position( Percent_Position{ 45, 55 } );
}

void Pause_Screen::input( sf::Event const& ev )
{
	if ( ev.type is sf::Event::KeyReleased )
		if ( ev.key.code is sf::Keyboard::Escape ) {
			G_App.toggle_pause();
			if ( G_App.is_paused() is false )
				G_Root.set_pause( false );
		}
}

void Pause_Screen::update( r32 dt )
{
	unused( dt );

	pause_title->visible = G_App.is_paused();
	option_resume->text->visible = G_App.is_paused();
	option_exit->text->visible = G_App.is_paused();
}
