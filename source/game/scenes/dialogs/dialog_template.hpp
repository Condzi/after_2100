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
	Signal<> s_dialog_finished;

	compile_constant DIALOG_TEXT_CHAR_SIZE    = 24;
	compile_constant CHARACTER_NAME_CHAR_SIZE = 20;
	compile_constant CHARACTER_LIMIT_PER_LINE = 67;

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
	s8 selected_response{ 1 };
	bool needs_update{ false };
	s32 character_height{ 0 };
	bool dialog_finished = false;

	Dialog_Template();

	void set_dialog( std::string_view dialog_name );
	void start();

	void input( sf::Event const& event ) override;

private:
	struct Dialog_Data final
	{
		struct Response final
		{
			std::string text, next;
		};

		std::string actor, text, next;
		std::vector<Response> responses;
	};

	nlohmann::json const* dialog_json;
	inline static nlohmann::json const* characters_data{ nullptr };
	Dialog_Data current_dialog_data;

	void add_responses();

	void update_responses_visibility();
	void update_arrow_position();

	Dialog_Data get_dialog_data( std::string const& id );

	void set_up_dialog( std::string const& id );
};