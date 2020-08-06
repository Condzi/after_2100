#pragma once

#include <engine/entity.hpp>
#include <engine/cstring.hpp>
#include <engine/logger.hpp>
#include <engine/utf8_string.hpp>

namespace con
{
// Test rendering of all characters.
struct Font_Test final
{
	ENTITY_DEF( Font_Test );


	void initialize()
	{
		auto& font = Context.prepared_resources->get_font( "dev_console"_hcs );
		
		auto [ri, size] = construct_text( L"Hej Nikola.\nZrobi³em wyœwietlanie tekstu.\nChyba dzia³a :D"_utf8, font, Text_Size::Developer_Console, -1 );
		
		_cold.basic_render_info = ri;
		_cold.basic_render_info.shader = Context.prepared_resources->get_shader( "text"_hcs );

		_hot.position.x += 200;
		_hot.position.y += 200;
		_hot.update_model_matrix = true;
		_cold.basic_render_info.tint.r = 120;

		con_log_indented( 3, "Created text, size: % x % px.", size.x, size.y );
	}

	void shutdown()
	{
		shutdown_textured_sprite( _cold.basic_render_info );
	}
};
}