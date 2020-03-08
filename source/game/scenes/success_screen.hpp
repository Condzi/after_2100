/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/
#pragma once

#include "framework/scene/clickable_label.hpp"
#include "framework/scene/rich_text.hpp"

using namespace con;

class Success_Screen final :
	public Node
{
	CLASS_DEF( Success_Screen );

public:
	Success_Screen();

private:
	Rich_Text* title_label;

	Clickable_Label* try_again_label;
	Clickable_Label* exit_label;
};