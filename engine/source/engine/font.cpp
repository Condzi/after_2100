#include <engine/font.hpp>

#include <engine/utility.hpp>
#include <engine/logger.hpp>
#include <engine/algorithms.hpp>

#include <algorithm>

namespace con
{
void Font::initialize( CString path, std::initializer_list<s8> text_sizes_ )
{
	auto& ta = reinterpret_cast<Temporary_Allocator&>( *Context.temporary_allocator );
	constant mark = ta.get_mark();
	defer{ ta.set_mark( mark ); };

	constant[file_data, file_loading_success] = load_entire_file_binary( path, Context.default_allocator );

	font_file_data = file_data;

	if ( !file_loading_success ) {
		return;
	}

	sft.font = sft_loadmem( font_file_data.data(), font_file_data.size() );

	if ( sft.font == nullptr ) {
		con_log_indented( 2, "Error: SFT can't load font from file \"%\".", path );
		return;
	}

	// We have to initialize all structure members. 
	// Setting offset to 0.
	sft.x = 0;
	sft.y = 0;
	// At first loop run we just measure the texture, so we're not setting
	// SFT_RENDER_IMAGE.
	sft.flags = SFT_DOWNWARD_Y | SFT_CATCH_MISSING;

	// We have to convert initializer list to our array here.
	// Maybe use stack_allocator here?
	text_sizes.initialize( text_sizes_.size(), Context.default_allocator );
	memcpy( text_sizes.data(), text_sizes_.begin(), text_sizes_.size() );

	// Get character info and calculate needed space for the texture.
	// We're looking for biggest height. The width is just all widths accumulated.
	s16 atlas_width  = -1;
	s16 atlas_height = -1;

	// Horizontal offset of current character inside the texture.
	// (we have one texture per one font size, hence we reset it in the outer loop)
	s32 offset_x = 0;

	SFT_Char sft_character;

	character_infos.initialize( text_sizes.size() );

	//
	// Find the atlas dimensions for each font size.
	//
	for ( s32 i = 0; i < text_sizes.size(); ++i ) {
		character_infos[i].initialize( alphabet.size );

		offset_x = 0;
		atlas_width = atlas_height = -1;
		constant size = Context.text_sizes[text_sizes[i]];

		sft.xScale = sft.yScale = size;

		for ( s32 j = 0; j < alphabet.size; ++j ) {
			// 0 means success.
			constant char_success_info = sft_char( &sft, alphabet.data[j], &sft_character );

			if ( char_success_info == -1 ) {
				con_log_indented( 3, "Error: can't load character for font \"%\". Alphabet idx = %, size = %.", path, j, size );
				continue;
			} else if ( char_success_info == 1 ) {
				con_log_indented( 3, "Warning: can't load character for font \"%\". Alphabet idx = %, size = %. Missing character will be used.", path, j, size );
			}

			auto& character_info = character_infos[i][j];
			character_info.width  = static_cast<s16>( sft_character.width );
			character_info.height = static_cast<s16>( sft_character.height );
			character_info.pen_offset.x = sft_character.x;
			character_info.pen_offset.y = sft_character.y;

			character_info.offset_in_texture = offset_x;

			atlas_height = std::max( atlas_height, character_info.height );
			atlas_width += character_info.width;
			offset_x += character_info.width;
		}

		// Generate gl texture. We'll fill it in the next loop.
		glGenTextures( 1, &textures[i] );
		glBindTexture( GL_TEXTURE_2D, textures[i] );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, atlas_width, atlas_height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}

	//
	// Generate the characters.
	//
	sft.flags |= SFT_RENDER_IMAGE;

	for ( s32 i = 0; i < text_sizes.size(); ++i ) {
		offset_x = 0;
		constant size = Context.text_sizes[text_sizes[i]];

		sft.xScale = sft.yScale = size;

		for ( s32 j = 0; j < alphabet.size; ++j ) {
			// 0 means success. -1 is failure, 1 is missing char.
			constant char_success_info = sft_char( &sft, alphabet.data[j], &sft_character );

			if ( char_success_info == -1 ) {
				continue;
			}

			auto& character_info = character_infos[i][j];

			glTexSubImage2D( GL_TEXTURE_2D, 0, offset_x, 0, character_info.width, character_info.height, GL_RED, GL_UNSIGNED_BYTE, character_info.image );

			free( sft_character.image );

			offset_x += character_info.width;
		}
	}
}

void Font::shutdown()
{
	sft_freefont( sft.font );
	font_file_data.shutdown();
	text_sizes.shutdown();

	glDeleteTextures( textures.size(), textures.data() );
	textures.shutdown();

	for ( s32 i = 0; i < character_infos.size(); ++i ) {
		character_infos[i].shutdown();
	}
	character_infos.shutdown();
}

returning Font::get_texture( s8 text_size ) -> gl_id
{
	constant result = linear_find( text_sizes, text_size );

	con_assert( result.found() );

	if ( result.not_found() ) {

		return 0;
	}

	return textures[result.idx];
}

returning con::Font::get_character_info( wchar_t character, s8 text_size ) -> Character_Info
{
	constant size_find_result = linear_find( text_sizes, text_size );
	con_assert( size_find_result.found() );

	if ( size_find_result.not_found() ) {
		con_log_indented( 2, "Error: can't find size in font. Char = %, size enum value = %.", static_cast<s32>( character ), text_size );

		return {};
	}

	// We don't have code to make array from UTF8_String, soo...
	s32 char_idx = -1;
	for ( s32 i = 0; i < alphabet.size; ++i ) {
		if ( alphabet[i] == character ) {
			char_idx = i;
			break;
		}
	}

	con_assert( char_idx != -1 );
	if ( char_idx == -1 ) {
		con_log_indented( 2, "Error: can't find character in font. Char = %, size enum value = %.", static_cast<s32>( character ), text_size );

		return {};
	}

	return character_infos[size_find_result.idx][char_idx];
}

returning con::Font::get_kerning( wchar_t left_character, wchar_t right_character, s8 text_size ) -> v2
{
	// check if we have text_size in our font
	// find text size idx
	// set the sft.xScale and yScale
	// asfsafssafsfefwaef
	return returning();
}
}