/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Font.hpp>

#include "node.hpp"

#include "framework/common/gui_string.hpp"

namespace con
{
class Rich_Text :
	public Node
{
	CLASS_DEF( Rich_Text );

public:
	GUI_String string;

	sf::Color fill_color{ sf::Color::White };
	sf::Color outline_color{ sf::Color::Black };
	s32 character_size{ 18 };
	r32 outline_thickness{ 1.0px };
	r32 line_spacing_factor{ 1 };
	r32 letter_spacing_factor{ 1 };
	bool visible{ true };
	s32 layer{ 0 };

	void set_font_from_name( std::string const& name );

	void update_vertices();

	void update( r32 dt ) override;
	void draw_gui( Drawing_Set& set ) override;

private:
	enum Style
	{
		Regular = 0,
		Bold    = 1 << 0,
		Italic  = 1 << 1,
	};

	sf::VertexArray vertices{ sf::Triangles };
	sf::VertexArray outline_vertices{ sf::Triangles };
	sf::Font const* font{ nullptr };

	sf::Transformable transformable;
	
	// shared between letters
	r32 line_spacing;
	r32 x, y;

	void add_character( u32 previous_character, u32 current_character, u32 style );
	void add_quad( sf::Glyph const& glyph, sf::Color const& color, r32 italic_shear = 0, r32 outline = 0 );
};
}