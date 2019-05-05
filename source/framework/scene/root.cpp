/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "root.hpp"
#include "application.hpp"

namespace con
{
void Root::handle_input( sf::Event const& event )
{
	if ( event.type == sf::Event::Closed )
		Application::get_instance().exit_game();
}

void Root::update( r32 delta )
{
	time_since_update += delta;

	if ( time_since_update >= fps_update_interval ) {
		time_since_update -= fps_update_interval;

		engine_log_info( "+++++ FPS: {0:.1f} +++++", 1 / delta );
	}
}
}
