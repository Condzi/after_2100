/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "rich_text.hpp"

#include "framework/common/resources_storage.hpp"

namespace con
{
void Rich_Text::set_font_from_name( std::string const& name )
{
	font = G_Resources_Storage.get_font( name );
}

void Rich_Text::update_vertices()
{
	report_warning_if( font is nullptr or string.get_string().isEmpty() )
		return;

	sf::String str = string.get_string();

	// 6 vertices of triangle make a quad
	vertices.resize( str.getSize() * 6 );
	outline_vertices.resize( str.getSize() * 6 );

	line_spacing = font->getLineSpacing( character_size ) * line_spacing_factor;
	x = 0;
	y = static_cast<r32>( character_size );

	u32 style = Style::Regular;

	u32 previous_character{ 0 }, current_character{ 0 };
	for ( size_t i = 0; i < str.getSize(); i++ ) {
		current_character = str[i];

		add_character( previous_character, current_character, style );

		previous_character = current_character;
	}
}

void Rich_Text::update( r32 dt )
{
	transformable.setPosition( get_global_position() );
	transformable.setScale( get_scale() );
	transformable.setRotation( get_rotation() );
}

void Rich_Text::draw_gui( Drawing_Set& set )
{
	if ( not visible or font is nullptr or string.get_string().isEmpty() )
		return;

	sf::RenderStates states;

	states.transform *= transformable.getTransform();
	states.texture = &font->getTexture( character_size );

	if ( outline_thickness is_not 0 )
		set.add_drawable( outline_vertices, layer, states );

	set.add_drawable( vertices, layer, states );
}
void Rich_Text::add_character( u32 previous_character, u32 current_character, u32 style )
{
	x += font->getKerning( previous_character, current_character, character_size );

	r32 whitespace_width = font->getGlyph( L' ', character_size, style&Style::Bold ).advance;
	constant letter_spacing = ( whitespace_width / 3.f ) * ( letter_spacing_factor - 1.f );
	whitespace_width += letter_spacing;

	if ( current_character == L' ' ) {
		x += whitespace_width;
		return;
	}

	if ( outline_thickness is_not 0 ) {
		constant& glyph = font->getGlyph( current_character, character_size, style & Style::Bold, outline_thickness );
		add_quad( glyph, outline_color, style & Style::Italic ? 0.209f : 0, outline_thickness );
	}

	constant& glyph = font->getGlyph( current_character, character_size, style & Style::Bold, 0 );
	add_quad( glyph, fill_color, style & Style::Italic ? 0.209f : 0 );

	// Advance to the next character
	x += glyph.advance + letter_spacing;
}

void Rich_Text::add_quad( sf::Glyph const& glyph, sf::Color const& color, r32 italic_shear, r32 outline )
{
	compile_constant padding = 1.0px;

	constant left = glyph.bounds.left - padding;
	constant top = glyph.bounds.top - padding;
	constant right = glyph.bounds.left + glyph.bounds.width + padding;
	constant bottom = glyph.bounds.top  + glyph.bounds.height + padding;

	float u1 = static_cast<float>( glyph.textureRect.left ) - padding;
	float v1 = static_cast<float>( glyph.textureRect.top ) - padding;
	float u2 = static_cast<float>( glyph.textureRect.left + glyph.textureRect.width ) + padding;
	float v2 = static_cast<float>( glyph.textureRect.top  + glyph.textureRect.height ) + padding;

	// if we have outline passed in we are adding outline vertices, if not then text vertices.
	sf::VertexArray& vert = outline != 0 ? outline_vertices : vertices;

	vert.append({ { x + left  - italic_shear * top    - outline, y + top    - outline }, color, { u1, v1 } } );
	vert.append({ { x + right - italic_shear * top    - outline, y + top    - outline }, color, { u2, v1 } } );
	vert.append({ { x + left  - italic_shear * bottom - outline, y + bottom - outline }, color, { u1, v2 } } );
	vert.append({ { x + left  - italic_shear * bottom - outline, y + bottom - outline }, color, { u1, v2 } } );
	vert.append({ { x + right - italic_shear * top    - outline, y + top    - outline }, color, { u2, v1 } } );
	vert.append({ { x + right - italic_shear * bottom - outline, y + bottom - outline }, color, { u2, v2 } } );
}
}