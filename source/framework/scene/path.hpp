/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#pragma once

#include "node.hpp"
#include "framework/common/signal.hpp"

#include <SFML/Graphics/VertexArray.hpp>

namespace con
{
class Path :
	public Node
{
	CLASS_DEF( Path );

public:
	std::vector<Point> points;

	// For debug:
	s32 layer{ 5 };
	sf::Color visual_representation_color{ sf::Color::Red };

	void draw( Drawing_Set& set ) override;

private:
	sf::VertexArray visual_representation{ sf::LinesStrip };
};

class Path_Follower :
	public Node
{
	CLASS_DEF( Path_Follower );

public:
	Signal<> s_on_finish_following;

	// If reached then go to next point.
	r32 minimum_distance{ 25.0px };
	r32 steering_force{ 200 };
	r32 steering_authority{ 0.06 };
	r32 max_velocity{ 210 };

	void start_following();
	void stop_following();

	auto get_velocity() const -> Vec2 const&;
	auto is_following() const -> bool;
	auto is_finished() const  -> bool;

	void set_path( Path const& path );

	void update( r32 dt ) override;

private:
	Path const* path_to_follow{ nullptr };
	Vec2        velocity;
	s32         current_target_id{ 0 };
	bool        following{ false };
};
}