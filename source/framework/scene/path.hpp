/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "node.hpp"

#include <SFML/Graphics/VertexArray.hpp>

namespace con
{
class Path :
	public Node
{
	CLASS_DEF( Path );


public:
	std::vector<Point> points;
	s32 layer{ 0 };
	bool draw_path{ false };

	auto calculate_length() const -> r32;

	void draw( Drawing_Set& set );

private:
	sf::VertexArray visual_representation{ sf::LinesStrip };
};

class Path_Follower :
	public Node
{
	CLASS_DEF( Path_Follower );

public:
	// If reached then go to next point.
	r32 minimum_distance{ 10.0px };
	r32 steering_force{ 100 };

	void start_following( r32 max_velocity_ );
	void stop_following();

	auto get_is_following() const -> bool;
	auto is_finished() const -> bool;

	void set_path( Path const& path );

	void update( r32 dt ) override;

private:
	r32 max_velocity{ 0 };
	const Path* path_to_follow{ nullptr };
	s32 current_target_id{ 0 };
	bool is_following{ false };
};
}