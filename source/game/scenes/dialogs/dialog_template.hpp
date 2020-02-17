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
	compile_constant DIALOG_TEXT_CHAR_SIZE    = 32;
	compile_constant CHARACTER_NAME_CHAR_SIZE = 20;
	compile_constant CHARACTER_LIMIT_PER_LINE = 50;

	Image* actor_image;
	Image* arrow_image;
	Rich_Text* dialog_text;
	Rich_Text* character_name;
	Panel* dialog_text_box;
	std::vector<Rich_Text*> response_text;
	Point response_text_begin_position;
	Point arrow_next_dialog_position;
	Point arrow_choice_response_position;
	bool show_responses{ false };
	s8 selected_response{ 0 };
	s32 current_dialog{ 0 };
	bool needs_update{ false };
	s32 character_height{ 0 };

	Dialog_Template();

	void add_responses( std::initializer_list<std::string> locs );

	void update_responses_visibility();
};