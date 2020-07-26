#pragma once

#include <engine/entity.hpp>
#include <engine/cstring.hpp>
#include <engine/logger.hpp>

#include <schrift/schrift.h>

namespace con
{
struct Font_Test final
{
	ENTITY_DEF( Font_Test );

	compile_constant string_to_display = L"Hello, world! œæŸ¿ó³ Æ¯Ó£ <3";
	compile_constant font_file         = CON_FONTS_FOLDER "oxanium.ttf";
	compile_constant text_size         = 32.0f;

	SFT_Font* sft_font = nullptr;

	void initialize()
	{
		_cold.basic_render_info.visible = false;
		con_log_indented( 2, "Loading font from %...", cstring_from_cstr( font_file ) );

		sft_font = sft_loadfile( font_file );

		if ( sft_font == nullptr ) {
			con_log_indented( 3, "Failed to open!" );
			return;
		}
	}

	void shutdown()
	{
		sft_freefont( sft_font );
	}
};
}