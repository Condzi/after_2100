/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "rich_text.hpp"

namespace con
{
class Clickable_Label :
	public Node
{
	CLASS_DEF( Clickable_Label );

public:
	enum Status
	{
		Idle,
		Hover
	};

	Signal<> s_label_clicked;

	sf::Color fill_color_idle{ sf::Color::White };
	sf::Color fill_color_hovered{ sf::Color{200, 200, 200} };

	sf::Color outline_color_idle{ sf::Color::Black };
	sf::Color outline_color_hovered{ sf::Color::Black };

	// @ToDo: Create normal text class and use it instead. (sfml one is meh tbh)
	Rich_Text* text{ nullptr };

	Clickable_Label();

	[[nodiscard]] auto get_status() const -> Status;

	void input( sf::Event const& event ) override;
	void update( r32 dt ) override;

private:
	Status status{ Status::Idle };
	bool text_needs_update{ true };

	void style_for_idle();
	void style_for_hover();
};
}
