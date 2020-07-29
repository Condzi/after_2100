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
	compile_constant atlas_height = 30;
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

		auto atlas_memory = Context.temporary_allocator->allocate( atlas_size_in_bytes );
		memset( atlas_memory, 0, atlas_size_in_bytes );

		con_log_indented( 2, "Generating characters..." );
		/*
		byte* next_free_slot = atlas_memory;

		for ( s32 i = 0; i < alphabet_length; ++i ) {
			constant success = sft_char( &sft, alphabet[i], &character );

			if ( success != 0 ) {
				con_log_indented( 3, "Failed to generate character of id %", i );
				continue;
			}


			memcpy( next_free_slot, character.image, character.width * character.height );
			next_free_slot += character.width * character.height;

			if ( next_free_slot >= atlas_memory + atlas_size_in_bytes ) {
				con_log_indented( 3, "Not enough memory for all characters! (% chars left to generate)", alphabet_length - i );
				break;
			}
		}
		con_log_indented( 2, "Memory left in the atlas: %", static_cast<s32>( atlas_memory + atlas_size_in_bytes - next_free_slot ) );
		*/

	
		// Check what happens for more than one character.
		// Why we had garbage in the texture?
		// Maybe add construct_text(wchar_t* str, font)?

		// the font is just storage for textures of given face with
		// various of sizes (maybe something like 16, 24, 32?)
		// Or maybe we should calculate the 3 main sizes depending on
		// the window resolution. Then we would update the fonts / texts
		// on every window resolution change.
		sft_char( &sft, L'ú', &character );

		glGenTextures( 1, &texture_id );
		glBindTexture( GL_TEXTURE_2D, texture_id );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, character.width, character.height, 0, GL_RED, GL_UNSIGNED_BYTE, character.image );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		//_cold.basic_render_info = construct_textured_sprite( atlas_width, atlas_height );

		_cold.basic_render_info = construct_textured_sprite( character.width, character.height );


		_cold.basic_render_info.shader = Context.prepared_resources->get_shader( "text"_hcs );
		_cold.basic_render_info.texture.id = texture_id;

		/*_cold.basic_render_info.shader = Context.prepared_resources->get_shader( "sprite_default"_hcs );
		*/
		/*
		_cold.basic_render_info = construct_textured_sprite( 48, 48 );
		_cold.basic_render_info.texture = Context.prepared_resources->get_texture( "player"_hcs );
		*/

		_hot.position = v2{ 100,100 };
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