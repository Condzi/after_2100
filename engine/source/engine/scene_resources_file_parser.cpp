#include <engine/scene_resources_file_parser.hpp>

#include <engine/logger.hpp>
#include <engine/assert.hpp>
#include <engine/utility.hpp>

#include <string>

namespace con
{
returning parse_scene_resources_file( CString path, Array<u32>& textures, Array<u32>& fonts, Array<u32>& shaders ) -> bool
{
	auto temporary_allocator = reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );
	constant mark = temporary_allocator->get_mark();
	defer{ temporary_allocator->set_mark( mark ); };

	//
	// Open the file and read its content.
	//

	constant[file_content, success] = load_entire_file_binary( path );

	if ( !success ) {
		return false;
	}

	compile_constant comment_mark = CString{ CON_CONFIG_COMMENT_MARK };
	compile_constant section_mark = CString{ CON_CONFIG_SECTION_MARK };

	// 
	// Get the data.
	//

	// We initialize this to the highest allowed value. Later we shrink them.
	textures.initialize( CON_MAX_SCENE_RESOURCES_ENTRIES, Context.default_allocator );
	fonts.initialize( CON_MAX_SCENE_RESOURCES_ENTRIES, Context.default_allocator );
	shaders.initialize( CON_MAX_SCENE_RESOURCES_ENTRIES, Context.default_allocator );

	// We initialize this to -1 to neatly use '++XXX' later. 
	s32 textures_idx = -1, fonts_idx = -1, shaders_idx = -1;

	enum Current_Section
	{
		Textures, Fonts, Shaders
	} current_section = Textures;

	for ( s32 idx = 0; idx < file_content.size(); ) {
		idx = ate_whitespace( file_content, idx );
		constant endline_idx = ate_chars_until( file_content, idx, '\n' );
		CString const temp{ file_content.data() + idx, endline_idx };

		if ( temp.begins_with( comment_mark ) ||
			 temp.size == 0 ) {
			idx = endline_idx;
			continue;
		} else if ( temp.begins_with( section_mark ) ) {
			// Using of hashed cstring here is possible, but probably not beneficial.
			CString const potential_section{ temp.data + section_mark.size, endline_idx };

			// @Robustness: We may want to remove magic strings like "textures" to macro_config

			if ( potential_section.begins_with( "textures" ) ) {
				current_section = Textures;
			} else if ( potential_section.begins_with( "fonts" ) ) {
				current_section = Fonts;
			} else if ( potential_section.begins_with( "shaders" ) ) {
				current_section = Shaders;
			} else {
				con_log_indented( 2, R"(Error: unknown resource section in "%": "%")", path, potential_section );
				return false;
			}

			idx = endline_idx;
			continue;
		}

		constant word_length = ate_chars_until_whitespace( file_content, idx ) - idx;
		CString const word{ temp.data, word_length };
		constant hash = hash_cstring( word );

		// @Robustness: check here if the hash matches the default resources one. If yes, then skip it.
		switch ( current_section ) {
		case Textures: textures[++textures_idx] = hash; break;
		case Fonts:    fonts[++fonts_idx]		= hash; break;
		case Shaders:  shaders[++shaders_idx]	= hash; break;
		}

		idx = endline_idx;
	}

	++textures_idx;
	++fonts_idx;
	++shaders_idx;

	con_log_indented( 2, "Entries found: % textures, % fonts, % shaders.", textures_idx, fonts_idx, shaders_idx );

	textures.shrink( textures_idx );
	fonts.shrink( fonts_idx );
	shaders.shrink( shaders_idx );

	return true;
}
}