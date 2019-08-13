/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "framework/scene/label.hpp"
#include "framework/scene/clickable_label.hpp"

using namespace con;

class Pause_Screen final :
	public Node
{
	CLASS_DEF( Pause_Screen );

public:
	Pause_Screen();

	void input( sf::Event const& ev ) override;
	void update( r32 dt ) override;

private:
	Label* pause_title;
	Clickable_Label* option_resume;
	Clickable_Label* option_exit;
};