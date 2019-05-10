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

	void input( sf::Event const& event ) override;
	void draw( Drawing_Set& set ) override;

private:
	sf::VertexArray visual_representation{ sf::LinesStrip };
};

class Path_Follower :
	public Node
{
	CLASS_DEF( Path_Follower );

public:
	// If reached then go to next point.
	r32 minimum_distance{ 50.0px };
	r32 steering_force{ 200 };
	r32 mass = 100;
	r32 max_velocity{ 200 };

	void start_following();
	void stop_following();

	auto get_velocity() const -> Vec2 const&;
	auto get_is_following() const -> bool;
	auto is_finished() const -> bool;

	void set_path( Path const& path );

	void update( r32 dt ) override;

private:
	Vec2 velocity;
	const Path* path_to_follow{ nullptr };
	s32 current_target_id{ 0 };
	bool is_following{ false };
};
}