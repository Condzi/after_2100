/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "framework/scene/node.hpp"

using namespace con;

class Pause_Screen final :
	public Node
{
	CLASS_DEF( Pause_Screen );

public:
	Pause_Screen();

	void input( sf::Event const& ev ) override;
};