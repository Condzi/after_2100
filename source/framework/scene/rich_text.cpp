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

auto Rich_Text::get_global_bounds() const -> Rectangle_Shape
{
	return { get_global_position(), bottom_right };
}

auto Rich_Text::find_character_position( size_t idx ) const -> Point
{
	report_warning_if ( idx * 6 > vertices.getVertexCount() )
		return Point::ZERO();

	Point position;

	if ( outline_thickness > 0 )
		position = outline_vertices[idx * 6].position;
	else
		position = vertices[idx * 6].position;

	return transformable.getTransform().transformPoint( position );
}

void Rich_Text::update_vertices()
{
	report_warning_if( font is nullptr or string.get_string().isEmpty() )
		return;

	copy_of_string = string.get_string();

	// @ToDo: This doesn't work because we should *reserve* instead of resize.
	// 6 vertices of triangle make a quad
	// vertices.resize( copy_of_string.getSize() * 6 );
	// outline_vertices.resize( copy_of_string.getSize() * 6 );
	bottom_right = Point::ZERO();

	line_spacing = font->getLineSpacing( character_size ) * line_spacing_factor;
	x = outline_thickness;
	y = static_cast<r32>( character_size ); // without / 1.25 the bounds are offseted at the top and not pixel-perfect
	bool bold{ false }, italic{ false };

	u32 previous_character{ 0 }, current_character{ 0 };
	bool want_to_escape{ false }; // % character (because json formatter is stupid and doesn't allow \)

	for ( size_t i = 0; i < copy_of_string.getSize(); i++ ) {
		current_character = copy_of_string[i];

		if ( not want_to_escape ) {
			// if any of these characters occur we will continue; to not render it.
			if ( current_character == L'#' ) {
				bold = !bold;
				continue;
			} else if ( current_character == L'_' ) {
				italic = !italic;
				continue;
			} else if ( current_character == L'\n' ) {
				y += line_spacing;
				x = outline_thickness;
				continue;
			} else if ( current_character == L'%' ) {
				want_to_escape = true;
				continue;
			}
		}

		want_to_escape = false;

		add_character( previous_character, current_character, bold, italic );

		previous_character = current_character;
	}

	update_tranform();

	log_info( "Vertices = {}", vertices.getVertexCount() );
}

void Rich_Text::update( r32 dt )
{
	unused( dt );

	update_tranform();
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

void Rich_Text::update_tranform()
{
	transformable.setPosition( get_global_position() );
	transformable.setScale( get_scale() );
	transformable.setRotation( get_rotation() );
}

void Rich_Text::add_character( u32 previous_character, u32 current_character, bool bold, bool italic )
{
	x += font->getKerning( previous_character, current_character, character_size );

	r32 whitespace_width = font->getGlyph( L' ', character_size, bold ).advance;
	constant letter_spacing = ( whitespace_width / 3.f ) * ( letter_spacing_factor - 1.f );
	whitespace_width += letter_spacing;

	if ( current_character == L' ' ) {
		x += whitespace_width;
		return;
	}

	if ( outline_thickness is_not 0 ) {
		constant& glyph = font->getGlyph( current_character, character_size, bold, outline_thickness );
		add_quad( glyph, outline_color, italic ? 0.209f : 0, outline_thickness );
	}

	constant& glyph = font->getGlyph( current_character, character_size, bold, 0 );
	add_quad( glyph, fill_color, italic ? 0.209f : 0 );

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

	vert.append( { { x + left  - italic_shear * top    - outline, y + top    - outline }, color, { u1, v1 } } );
	vert.append( { { x + right - italic_shear * top    - outline, y + top    - outline }, color, { u2, v1 } } );
	vert.append( { { x + left  - italic_shear * bottom - outline, y + bottom - outline }, color, { u1, v2 } } );
	vert.append( { { x + left  - italic_shear * bottom - outline, y + bottom - outline }, color, { u1, v2 } } );
	vert.append( { { x + right - italic_shear * top    - outline, y + top    - outline }, color, { u2, v1 } } );
	vert.append( { { x + right - italic_shear * bottom - outline, y + bottom - outline }, color, { u2, v2 } } );

	// the last added vertex is the bottom_right one
	bottom_right = Point{ std::max( bottom_right.x, x + right - italic_shear * bottom - outline ), std::max( bottom_right.y, y + bottom - outline ) };
}
}