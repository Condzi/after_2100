/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/assertions.hpp"
#include "root.hpp"
#include "application.hpp"

namespace con
{
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

	if ( time_since_update >= fps_update_interval ) {
		time_since_update -= fps_update_interval;

		engine_log_info( "+++++ FPS: {0:.1f} +++++", 1 / delta );
	}
}

void Root::change_scene( Node_Ptr&& scene_to_switch )
{
	report_error_if( scene_to_switch is nullptr )
	{
		return;
	}

	report_warning_if( pending_scene is_not nullptr )
	{
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
