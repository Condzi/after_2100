/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include <SFML/Window/Event.hpp>

#include "framework/common/drawing_set.hpp"

#include "path.hpp"

namespace con
{
void Path::input( sf::Event const& event )
{
	if ( event.type == sf::Event::KeyReleased )
		if ( event.key.code == sf::Keyboard::F1 )
			draw_path = !draw_path;
}

void Path::draw( Drawing_Set& set )
{
	if ( not draw_path )
		return;

	visual_representation.resize( points.size() );

	for ( size_t i = 0; i < points.size(); i++ )
		visual_representation[i] = sf::Vertex{ points[i], visual_representation_color };

	set.add_drawable( visual_representation, layer );
}

void Path_Follower::start_following()
{
	report_warning_if( is_following );

	report_error_if( path_to_follow is nullptr )
	{
		return;
	}
	report_error_if( path_to_follow->points.empty() )
	{
		return;
	}

	set_global_position( path_to_follow->points.front() );

	is_following = true;
}

void Path_Follower::stop_following()
{
	report_warning_if( is_following is false );

	velocity = Vec2::ZERO();
	is_following = false;
}

auto Path_Follower::get_velocity() const -> Vec2 const &
{
	return velocity;
}

auto Path_Follower::get_is_following() const -> bool
{
	return is_following;
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
	if ( not is_following )
		return;

	report_error_if( path_to_follow is nullptr )
	{
		stop_following();
		return;
	}

	report_error_if( max_velocity <= 0 )
	{
		engine_log_error( "'max_velocity' has to be positive, like you :)" );
		stop_following();
		return;
	}

	constant target_position{ path_to_follow->points[current_target_id] };
	constant current_position{ get_global_position() };

	if ( current_position.distance( target_position ) <= minimum_distance )
		current_target_id++;

	if ( is_finished() )
	{
		is_following = false;
		s_on_finish_following.notify();
	}

	constant desired_velocity = Vec2{ target_position - current_position }.normalize() * max_velocity;
	constant steering = truncate( desired_velocity - velocity, steering_force ) * steering_authority;

	velocity = truncate( velocity + steering, max_velocity );

	move( velocity * dt );
}
}