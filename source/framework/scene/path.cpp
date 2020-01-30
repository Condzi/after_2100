/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include <SFML/Window/Event.hpp>

#include "framework/common/debug_flags.hpp"
#include "framework/common/drawing_set.hpp"

#include "path.hpp"

namespace con
{
void Path::draw( Drawing_Set& set )
{
	static constant& draw_paths = G_Debug_Flags.get( "draw_paths" );

	if ( draw_paths is false )
		return;

	visual_representation.resize( points.size() );

	for ( size_t i = 0; i < points.size(); i++ )
		visual_representation[i] = sf::Vertex{ points[i], visual_representation_color };

	set.add_drawable( visual_representation, layer );
}

void Path_Follower::start_following()
{
	report_warning_if( following );

	report_error_if( path_to_follow is nullptr )
	{
		return;
	}
	report_error_if( path_to_follow->points.empty() )
	{
		return;
	}

	set_global_position( path_to_follow->points.front() );

	current_target_id = 0;
	following = true;
}

void Path_Follower::stop_following()
{
	report_warning_if( following is false );

	velocity = Vec2::Zero();
	following = false;
}

auto Path_Follower::get_velocity() const -> Vec2 const&
{
	return velocity;
}

auto Path_Follower::is_following() const -> bool
{
	return following;
}

auto Path_Follower::is_finished() const -> bool
{
	return ( current_target_id is path_to_follow->points.size() );
}

void Path_Follower::set_path( Path const& path )
{
	path_to_follow = &path;
}

void Path_Follower::update( r32 dt )
{
	if ( not following )
		return;

	report_error_if( path_to_follow is nullptr )
	{
		stop_following();
		return;
	}

	if ( max_velocity <= 0 ) {
		engine_log_error( "'max_velocity' has to be positive, like you :)" );
		stop_following();
		return;
	}

	constant target_position{ path_to_follow->points[current_target_id] };
	constant current_position{ get_global_position() };

	if ( current_position.distance( target_position ) <= minimum_distance )
		current_target_id++;

	if ( is_finished() ) {
		following = false;
		s_on_finish_following.emit();
	}

	constant desired_velocity = Vec2{ target_position - current_position }.normalize() * max_velocity;
	constant steering = truncate( desired_velocity - velocity, steering_force ) * steering_authority;

	velocity = truncate( velocity + steering, max_velocity );

	move( velocity * dt );
}
}