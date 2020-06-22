#include <engine/scene_resources_file_parser.hpp>

#include <engine/logger.hpp>
#include <engine/assert.hpp>

#include <filesystem>
#include <fstream>
#include <string>

namespace con
{
// @Robustness: It's redundant with config_file.cpp content -- move it to different file.
// For now it'll do.
file_scope
{
returning ate_chars_until( Array<char>& arr, s32 current_idx, char c ) -> s32
{
	s32 idx = current_idx;
	for ( ; idx != arr.size() && arr[idx] != c; ++idx );
	return idx;
}

returning ate_chars_until_whitespace( Array<char>& arr, s32 current_idx ) -> s32
{
	s32 idx = current_idx;
	for ( ; idx != arr.size() && ::isspace( arr[idx] ) == 0; ++idx );
	return idx;
}

returning ate_whitespace( Array<char>& arr, s32 current_idx ) -> s32
{
	s32 idx = current_idx;
	for ( ; idx != arr.size() && ::isspace( arr[idx] ) != 0; ++idx );
	return idx;
}
}

returning parse_scene_resources_file( CString path, Array<CString>& textures, Array<CString>& fonts, Array<CString>& shaders ) -> bool
{
	auto temporary_allocator = reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );
	constant mark = temporary_allocator->get_mark();
	defer{ temporary_allocator->set_mark( mark ); };

	std::ifstream input( std::string{ path.data, path.data + path.size }, std::ios::binary );
	if ( input.is_open() == false ) {
		con_log_indented( 1, R"(Error opening file "%".)", path );
		return false;
	}

	std::error_code fs_error_code;
	constant file_size = static_cast<s32>( std::filesystem::file_size( { path.data, path.data + path.size }, fs_error_code ) );
	if ( fs_error_code ) {
		con_log_indented( 1, R"(Error reading file size for "%", info: "%")", path, cstring_from_stdstring( fs_error_code.message() ) );
		return false;
	}

	Array<char> file_content;
	file_content.initialize( file_size, temporary_allocator );
	input.read( file_content.data(), file_size );
	input.close();

	// @Robustness: This may be an actual use case for growing array like vector.

	//
	// Count entries.
	//
	compile_constant comment_mark = CString{ CON_CONFIG_COMMENT_MARK };
	compile_constant section_mark = CString{ CON_CONFIG_SECTION_MARK };

	s32 textures_entries = 0, fonts_entries = 0, shaders_entries = 0;
	enum Current_Section
	{
		Textures, Fonts, Shaders
	} current_section = Textures;

	for ( s32 idx = 0; idx < file_size; ) {
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
				con_log_indented( 1, R"(Error: unknown resource section in "%": "%")", path, potential_section );
				return false;
			}

			idx = endline_idx;
			continue;
		}

		if ( current_section == Textures ) {
			++textures_entries;
		} else if ( current_section == Fonts ) {
			++fonts_entries;
		} else if ( current_section == Shaders ) {
			++shaders_entries;
		}

		idx = endline_idx;
	}

	con_log_indented( 1, "Entries count found: % textures, % fonts, % shaders.", textures_entries, fonts_entries, shaders_entries );
	textures.initialize( textures_entries );
	fonts.initialize( fonts_entries );
	shaders.initialize( shaders_entries );

	//
	// Actually getting the data.
	//

	s32 textures_idx = -1, fonts_idx = -1, shaders_idx = -1;

	for ( s32 idx = 0; idx < file_size; ) {
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
				con_log_indented( 1, R"(Error: unknown resource section in "%": "%")", path, potential_section );
				return false;
			}

			idx = endline_idx;
			continue;
		}

		constant word_length = ate_chars_until_whitespace( file_content, idx ) - idx;
		CString const word{ temp.data, word_length };

		switch ( current_section ) {
		case Textures: textures[++textures_idx] = word; break;
		case Fonts:    fonts[++fonts_idx] = word;       break;
		case Shaders:  shaders[++shaders_idx] = word;   break;
		}

		idx = endline_idx;
	}

	con_assert( textures_idx == textures_entries-1 );
	con_assert( fonts_idx == fonts_entries-1 );
	con_assert( shaders_idx == shaders_entries-1 );

	return true;
}
}