/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "framework/common/drawing_set.hpp"

#include "path.hpp"

namespace con
{
auto Path::calculate_length() const -> r32
{
	r32 length{ 0 };

	Point previous_point;

	for ( constant& point : points ) {
		length += previous_point.distance( point );
		previous_point = point;
	}

	return length;
}

void Path::draw( Drawing_Set& set )
{
	if ( not draw_path )
		return;

	visual_representation.resize( points.size() );

	for ( size_t i = 0; i < points.size(); i++ )
		visual_representation[i] = sf::Vertex{ points[i], sf::Color::Red };

	set.add_drawable( visual_representation, layer );
}

void Path_Follower::start_following( r32 max_velocity_ )
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
	report_error_if( max_velocity_ <= 0 )
	{
		return;
	}

	// v = s*t
	max_velocity = max_velocity_;

	set_global_position( path_to_follow->points.front() );

	is_following = true;
}

void Path_Follower::stop_following()
{
	report_warning_if( is_following is false );

	is_following = false;
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
		return;
	}

	constant target_position{ path_to_follow->points[current_target_id] };
	constant current_position{ get_global_position() };

	if ( current_position.distance( target_position ) <= minimum_distance )
		current_target_id++;

	if ( is_finished() )
		is_following = false;

	constant desired_velocity = Vec2{ target_position - current_position }.normalize() * max_velocity;
	constant steering = truncate( desired_velocity - current_velocity, steering_force ) * ( 1/mass );

	current_velocity = truncate( current_velocity + steering, max_velocity );

	move( current_velocity * dt );
}
}