/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/assertions.hpp"
#include "framework/common/debug_flags.hpp"
#include "framework/scene/debug_console.hpp"

#include "application.hpp"
#include "root.hpp"

namespace con
{
Root::Root()
{
	name = "root";
	fps_label = attach<Rich_Text>();
	fps_label->name = "fps_label";
	fps_label->string.set_locale_name( "#loc_fps" );
	fps_label->set_absolute_position( Percent_Position{ 1.0,1.0 } );
	fps_label->string.update_locale_parameters( 0.0 );
}

void Root::input( sf::Event const& event )
{
	if ( event.type is sf::Event::Closed )
		G_App.exit_game();
}

void Root::update( r32 delta )
{
	G_Profile_Function();
	static constant& display_fps = G_Debug_Flags.get( "display_fps" );

	fps_label->visible = display_fps;

	if ( display_fps is false )
		return;

	time_since_update += delta;

	if ( time_since_update >= FPS_UPDATE_INTERVAL ) {
		time_since_update -= FPS_UPDATE_INTERVAL;

		fps_label->string.update_locale_parameters( 1/delta );
		fps_label->update_vertices();
	}
}
}