/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#pragma once

#include "framework/scene/rich_text.hpp"

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

private:
	r32        time_since_update{ 0 };
	Rich_Text* fps_label;

	compile_constant FPS_UPDATE_INTERVAL = 0.1sec;
};
}