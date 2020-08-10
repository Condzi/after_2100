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
		
		/*
		auto& font = Context.prepared_resources->get_font( "dev_console"_hcs );
		
		auto [ri, size] = construct_text( L"tab =\t4 spaces\nnew line!\nanother one :)"_utf8, font, Text_Size::Developer_Console, 40 );
		
		ri.drawing_group = Drawing_Group::GUI;
		_cold.render_info = ri;

		//_hot.position.x += 200;
		//_hot.position.y += 200;
		_hot.update_model_matrix = true;
		_cold.render_info.tint.r = 120;
		*/
		
		
	 

		
		_cold.render_info = construct_rectangle( 200, 400 );
		_cold.render_info.tint ={ 218, 245, 235, 100 };
		_cold.render_info.shader = Context.prepared_resources->get_shader( "geometry_tinted"_hcs );

		_hot.position = v2( 500, 400 );
		_hot.update_model_matrix = true;
		
		
		
	}

	void shutdown()
	{
		shutdown_rectangle( _cold.render_info );
	}
};
}