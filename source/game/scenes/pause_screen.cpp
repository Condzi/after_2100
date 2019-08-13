/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "pause_screen.hpp"

#include "framework/scene/application.hpp"
#include "framework/scene/label.hpp"

Pause_Screen::Pause_Screen()
{
	auto label = attach<Label>();

	label->string.set_locale_name( "#loc_pause" );
	label->set_absolute_position( Percent_Position{ 45,20 } );
	bond_disconnector( label->s_update.connect( [=]( r32 dt ) {
		unused( dt );
		label->visible = G_App.is_paused();
					   } ) );
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
