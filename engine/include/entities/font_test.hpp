#pragma once

#include <engine/entity.hpp>
#include <engine/cstring.hpp>
#include <engine/logger.hpp>
#include <engine/utf8_string.hpp>

namespace con
{
struct Font_Test final
{
	ENTITY_DEF( Font_Test );


	void initialize()
	{
		auto& font = Context.prepared_resources->get_font( "dev_console"_hcs );
		
		auto [ri, size] = construct_text( L"tab =\t4 spaces\nnew line!\nanother one :)"_utf8, font, Text_Size::Developer_Console, 40 );
		
		ri.drawing_group = Drawing_Group::GUI;
		_cold.render_info = ri;

		//_hot.position.x += 200;
		//_hot.position.y += 200;
		_hot.update_model_matrix = true;
		_cold.render_info.tint.r = 120;

		con_log_indented( 3, "Created text, size: % x % px.", size.x, size.y );
	}

	void shutdown()
	{
		shutdown_textured_sprite( _cold.render_info );
	}
};
}