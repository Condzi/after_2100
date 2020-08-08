#pragma once

#include <engine/array.hpp>
#include <engine/render_info.hpp>
#include <engine/font.hpp>

namespace con
{

struct Textured_Vertex2D final
{
	v2 position;
	v2 texture_point;
};

// For debug geometry. Use with Render_Type::Draw_Arrays
struct Position_Vertex2D final
{
	v2 position;
};

//
// Quads have vertices in the following order:
//
//				0-----3
//				|     |
//				|     |
//				1-----2
//


//
// Rendering stages:
// 0. (rarely) update projection_view_multiplied_matrix.
// 1. Loop over entities in Entity Manager and gather necessary drawing data (Render_Info + entity type?)
// 2. Sort by layer.
// 3. Draw. (set the shader uniform variables according to entity type and shader type)
//
class Renderer final
{
public:
	void initialize();
	void shutdown();

	// Use this ebo if you're constructing a quad.
	returning get_quad_ebo() const -> gl_id const;

	// Should be called only by the window to update our internal
	// projection matrix. (called by application during initialization stage)
	void set_window_size( s32 width, s32 height );

	void render();

private:
	mat4 projection_matrix;
	mat4 view_matrix{ 1 }; // identity for now. @ToDo: make working camera.
	mat4 projection_view_multiplied_matrix;

	bool update_projection_view_multiplied_matrix = true;

	gl_id quad_ebo = 0;
};


//
// Functions useful to generate rendering objects. Use them to create
// actual OpenGL primitives.
// They're allocating using temporary allocator.
//

// In future we may want to use texture atlas to hold sprites.
// Then, it'll be convinient to use dedicated functions with this
// structure as parameter and 's32 texture_width, s32 texture_height`
// to generate desired sprite.
/*
struct Texture_Rect final
{
	s32 x = 0;
	s32 y = 0;
	s32 width = 0;
	s32 height = 0;
};
*/

// Generates points for rectangle with middle in (0,0) and specified
// size (in pixels, not the OpenGL coord space)
returning construct_2d_textured_quad( s32 width, s32 height ) -> Array<Textured_Vertex2D>;

// Works similar to `construct_2d_textured_quad`, but uses different 
// vertex type.
returning construct_2d_quad( s32 width, s32 height ) -> Array<Position_Vertex2D>;

// Creates a render info set up for sprite rendering. You still have to set the correct texture
// and shader.
returning construct_textured_sprite( s32 width, s32 height ) -> Render_Info;
// Just deletes vertex array and vertex buffer.
void shutdown_textured_sprite( Render_Info const& render_info );

// It's nice to know how big the text is.
struct Text_Return_Value final
{
	Render_Info render_info;
	v2 size;
};

// You can set line_length_limit to -1 to skip that feature.
// text_size should be value of Text_Size::X enum. It should also be avaible in the font.
returning construct_text( UTF8_String utf8_string, Font& font, s8 text_size, s16 line_length_limit = 128 ) -> Text_Return_Value;

void shutdown_text( Render_Info const& render_info );

returning construct_rectangle( s32 width, s32 height ) -> Render_Info;

void shutdown_rectangle( Render_Info const& render_info );
}