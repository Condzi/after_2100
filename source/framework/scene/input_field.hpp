/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "Rich_Text.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

namespace con
{
class Input_Field final :
	public Node
{
	CLASS_DEF( Input_Field );

public:
	// When user updates input field this is emitted with parent Input_Field as pointer.
	Signal<Input_Field const* const> s_input_changed;

	s32  layer{ 0 };
	bool visible{ true };

	Input_Field();
	
	[[nodiscard]] auto get_input_string() const -> std::string const&;

	void set_background_color_default( sf::Color const& color );
	void set_background_color_selected( sf::Color const& color );
	void set_text_color_default( sf::Color const& color );
	void set_text_color_input( sf::Color const& color );
	void set_default_string( std::string const& str );
	void set_character_limit( s32 limit );
	void set_character_size( s32 size );

	void input( sf::Event const& event ) override;
	void update( r32 dt ) override;
	void draw_gui( Drawing_Set& drawing_set ) override;

private:
	enum
	{
		Idle, Selected
	} status = IDLE;

	sf::Color background_color_default;
	sf::Color background_color_selected;

	sf::Color text_color_default;
	sf::Color text_color_input;

	std::string input_string;
	std::string default_string;

	s32 character_limit{ 4 };

	sf::RectangleShape background_shape;
	Rich_Text* input_text;
};
}