/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/
#pragma once

#include "framework/scene/image.hpp"
#include "framework/scene/rich_text.hpp"
#include "framework/scene/panel.hpp"

using namespace con;

class Dialog_Template :
	public Node
{
	CLASS_DEF( Dialog_Template );

public:
	Image* actor_image;
	Image* arrow_image;
	Rich_Text* dialog_text;
	Rich_Text* character_name;
	Panel* dialog_text_box;
	std::vector<std::string> dialog_locs; // Displaying one loc at the time using dialog_text.
	std::vector<Rich_Text*> response_text;
	Point response_text_begin_position;
	bool show_responses{ false };
	s8 selected_response{ 0 };
	s32 current_dialog{ 0 };
	bool needs_update{ false };

	Dialog_Template();

	void update_responses_visibility();
};