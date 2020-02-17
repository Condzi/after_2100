/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/
#pragma once

#include "dialog_template.hpp"

using namespace con;

class Dialog_Tutorial :
	public Dialog_Template
{
	CLASS_DEF( Dialog_Tutorial );

public:
	Dialog_Tutorial();

	void input( sf::Event const& event ) override;
	void update( r32 dt ) override;
};
