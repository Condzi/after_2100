#pragma once

#include <engine/entity.hpp>
#include <engine/cstring.hpp>
#include <engine/logger.hpp>

#include <schrift/schrift.h>

namespace con
{
// Test rendering of all characters.
struct Font_Test final
{
	ENTITY_DEF( Font_Test );

	compile_constant string_to_display = L"Hello, world! úÊüøÛ≥ è∆Øè”£";
	compile_constant font_file         = CON_FONTS_FOLDER "oxanium.ttf";
	compile_constant text_size         = 32.0f;
	compile_constant alphabet		   = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" // Latin chars
		L"ó!?()[]@#%*:;.,=<>" // '-' It's a dash, not a minus!! (U+2014)
		L"•∆ £—”åèØπÊÍ≥ÒÛúüø"; // Polish characters
	compile_constant alphabet_length   = 89;

	// @ToDo: For every used size have corresponding "safe" values.
	// Check them in Bitmap Font Generator program.
	compile_constant atlas_width  = 1700;
	compile_constant atlas_height = 40;
	compile_constant atlas_size_in_bytes = atlas_width * atlas_height;

	SFT sft;

	gl_id texture_id = 0;

	void initialize()
	{
		con_log_indented( 2, "Loading font from %...", cstring_from_cstr( font_file ) );

		sft.font = sft_loadfile( font_file );

		if ( sft.font == nullptr ) {
			con_log_indented( 3, "Failed to open!" );
			return;
		}

		sft.xScale = text_size;
		sft.yScale = text_size;
		sft.x = 0;
		sft.y = 0;


		sft.flags = SFT_DOWNWARD_Y | SFT_RENDER_IMAGE | SFT_CATCH_MISSING;
		SFT_Char character;

		con_log_indented( 2, "Generating characters..." );

		glGenTextures( 1, &texture_id );
		glBindTexture( GL_TEXTURE_2D, texture_id );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, atlas_width, atlas_height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		s32 offset_x = 0;
		for ( s32 i = 0; i < alphabet_length; ++i ) {
			constant success = sft_char( &sft, alphabet[i], &character );

			if ( success != 0 ) {
				con_log_indented( 3, "Failed to generate character of id %", i );
				continue;
			}
			
			con_assert( character.height < atlas_height );

			glTexSubImage2D( GL_TEXTURE_2D, 0, offset_x, 0, character.width, character.height, GL_RED, GL_UNSIGNED_BYTE, character.image );

			offset_x += character.width;
			free( character.image );
		}




		_cold.basic_render_info = construct_textured_sprite( atlas_width, atlas_height );


		_cold.basic_render_info.shader = Context.prepared_resources->get_shader( "text"_hcs );
		_cold.basic_render_info.texture.id = texture_id;

		_hot.position = v2{ 500,100 };
		_hot.update_model_matrix = true;
		_cold.basic_render_info.render_type = Render_Type::Draw_Elements;
		_cold.basic_render_info.drawing_layer = 5;
	}

	void shutdown()
	{
		glDeleteTextures( 1, &texture_id );
		shutdown_textured_sprite( _cold.basic_render_info );
		sft_freefont( sft.font );
	}
};
}