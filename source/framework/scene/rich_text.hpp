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
	void set_font_from_pointer( sf::Font const* ptr );

	[[nodiscard]] auto get_global_bounds() const -> Rectangle_Shape;
	[[nodiscard]] auto find_character_position( size_t idx ) const -> Point;
	[[nodiscard]] auto get_string_without_formating_characters() const -> sf::String const&;

	void update_vertices();

	void update( r32 dt ) override;
	void draw_gui( Drawing_Set& set ) override;

private:
	sf::VertexArray vertices{ sf::Triangles };
	sf::VertexArray outline_vertices{ sf::Triangles };
	sf::Font const* font{ nullptr };
	sf::String string_without_formatting_characters;

	sf::Transformable transformable;

	// shared between letters
	r32 line_spacing;
	r32 x, y;

	// bounds, top left one is always 0.0
	Point bottom_right; // = size of the rectangle, actually

	void update_tranform();
	void add_character( u32 previous_character, u32 current_character, bool is_bold, bool is_italic );
	void add_quad( sf::Glyph const& glyph, sf::Color const& color, r32 italic_shear = 0, r32 outline = 0 );
};
}