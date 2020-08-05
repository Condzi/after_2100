#pragma once

#include <engine/entity.hpp>
#include <engine/cstring.hpp>
#include <engine/logger.hpp>

namespace con
{
// Test rendering of all characters.
struct Font_Test final
{
	ENTITY_DEF( Font_Test );


	void initialize()
	{
		auto& font = Context.prepared_resources->get_font( "dev_console"_hcs );
		constant texture_id = font.get_texture( Text_Size::Developer_Console );

		s32 tex_w = -1, tex_h = -1;
		glBindTexture( GL_TEXTURE_2D, texture_id );
		glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tex_w );
		glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tex_h );

		_cold.basic_render_info = construct_textured_sprite( tex_w, tex_h );

		_cold.basic_render_info.shader = Context.prepared_resources->get_shader( "text"_hcs );
		_cold.basic_render_info.texture.id = texture_id;

		_hot.position = v2{ 1366/2, 768 / 2 };
		_hot.update_model_matrix = true;
		_cold.basic_render_info.render_type = Render_Type::Draw_Elements;
		_cold.basic_render_info.drawing_layer = 5;
	}

	void shutdown()
	{
		shutdown_textured_sprite( _cold.basic_render_info );
	}
};
}