/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#pragma once

#include "node.hpp"

#include <SFML/Graphics/CircleShape.hpp>

namespace con
{
class Root final :
	public Node
{
	CLASS_DEF( Root );

	friend class Application;

public:
	Root();

	void input( sf::Event const& event ) override;
	void update( r32 delta ) override;

	void change_scene( Node_Ptr&& scene );

private:
	Node*    active_scene{ nullptr };
	Node_Ptr pending_scene{ nullptr };

	r32          time_since_update{ 0 };
	class Label* fps_label;

	static constexpr r32 FPS_UPDATE_INTERVAL{ 0.1sec };
};
}