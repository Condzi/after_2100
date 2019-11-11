/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/assertions.hpp"
#include "framework/common/debug_flags.hpp"
#include "framework/scene/debug_console.hpp"

#include "application.hpp"
#include "label.hpp"
#include "root.hpp"

namespace con
{
Root::Root()
{
	fps_label = attach<Label>();
	fps_label->name = "fps_label";
	fps_label->string.set_locale_name( "#loc_fps" );
	fps_label->set_absolute_position( Percent_Position{ 1.0,1.0 } );
}

void Root::input( sf::Event const& event )
{
	if ( event.type is sf::Event::Closed )
		G_App.exit_game();
}

void Root::update( r32 delta )
{
	fps_label->visible = G_Debug_Flags.get( "display_fps" );

	time_since_update += delta;

	if ( time_since_update >= FPS_UPDATE_INTERVAL ) {
		time_since_update -= FPS_UPDATE_INTERVAL;

		fps_label->string.update_locale_parameters( 1/delta );
	}
}
}