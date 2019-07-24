/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/assertions.hpp"

#include "root.hpp"
#include "application.hpp"
#include "label.hpp"

namespace con
{
Root::Root()
{
	fps_label = attach<Label>();
	fps_label->name = "fps_label";
	fps_label->string.set_ascii( "~.~fps" );
	fps_label->set_absolute_position( Percent_Position{ 1.0,1.0 } );

	bond_disconnector( s_update.connect(
		[this]( r32 dt ) {
			unused( dt );
			fps_label->visible = G_Debug_Flags.display_fps;
		} ) );
}

void Root::input( sf::Event const& event )
{
	if ( event.type is sf::Event::Closed )
		G_App.exit_game();
}

void Root::update( r32 delta )
{
	if ( active_scene is nullptr and pending_scene is_not nullptr )
		active_scene = attach( change_owner( pending_scene ) );

	time_since_update += delta;

	if ( time_since_update >= FPS_UPDATE_INTERVAL ) {
		time_since_update -= FPS_UPDATE_INTERVAL;

		fps_label->string.set_ascii( fmt::format( "{0:.1f} fps", 1/delta ) );
}
}

void Root::change_scene( Node_Ptr&& scene_to_switch )
{
	report_error_if( scene_to_switch is nullptr )
	{
		return;
	}

	if ( pending_scene is_not nullptr ) {
		engine_log_warning( "There was one scene that was going to change to ({}). Changing to: {}.", pending_scene->name, scene_to_switch->name );
	}

	if ( active_scene is nullptr ) {
		active_scene = attach( change_owner( scene_to_switch ) );
		return;
	}

	engine_log_info( "Scene transition: \"{}\" => \"{}\".", active_scene->name, scene_to_switch->name );
	pending_scene = change_owner( scene_to_switch );
	active_scene->queue_for_delete();
	active_scene = nullptr;
}
}
