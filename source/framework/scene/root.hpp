/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "node.hpp"

namespace con
{
class Root final :
	public Node
{
	CLASS_DEF( Root );

	friend class Application;

public:
	void handle_input( sf::Event const& event ) override;
	void update( r32 delta ) override;

private:
	r32 time_since_update{ 0 };
	// in seconds
	static constexpr r32 fps_update_interval{ 1 };
};
}