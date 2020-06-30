#include <engine/config_file.hpp>
#include <engine/allocator.hpp>
#include <engine/logger.hpp>

#include <cstring>
#include <filesystem>
#include <fstream>

namespace con
{
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

returning ate_whitespace_reversed( Array<char>& arr, s32 current_idx ) -> s32
{
	s32 idx = current_idx;
	for ( ; idx != 0 && ::isspace( arr[idx] ) != 0; --idx );
	return idx;
}

returning ate_whitespace( Array<char>& arr, s32 current_idx ) -> s32
{
	s32 idx = current_idx;
	for ( ; idx != arr.size() && ::isspace( arr[idx] ) != 0; ++idx );
	return idx;
}
}

returning Config_File::parse_from_file( CString path ) -> bool
{
	con_assert( config_values.size() <= 0 ); // only one config file per parse for now.

	// We're allocating stuff here using temporary allocator.
	auto temporary_allocator = reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );
	constant mark = temporary_allocator->get_mark();
	defer{ temporary_allocator->set_mark( mark ); };

	//
	// Get file size, open it and copy its content.
	//

	std::error_code fs_error_code;
	constant file_size = static_cast<s32>( std::filesystem::file_size( cstring_to_stdsv( path ), fs_error_code ) );

	if ( fs_error_code ) {
		con_log_indented( 1, R"(Error: can't get file size for "%". Error message: "%".)", path, cstring_from_stdstring( fs_error_code.message() ) );
	}

	std::ifstream input( cstring_to_stdsv( path ), std::ios::binary );

	if ( !input.is_open() ) {
		con_log_indented( 1, R"(Error: can't open file "%".)", path );
		return false;
	}

	Array<char> file_content;
	file_content.initialize( file_size, temporary_allocator );
	input.read( file_content.data(), file_size );
	input.close();

	//
	// Get the config entries.
	//

	config_values.initialize( CON_MAX_CONFIG_ENTRIES, Context.default_allocator );

	u32 current_section_hash = 0;
	// Count lines which aren't empty, aren't a comment and aren't a section beginning.
	// We use this value to determine how much memory we need for `config_values`.
	s32 current_config_value = 0;

	for ( s32 idx = 0; idx < file_content.size(); ) {
		idx = ate_whitespace( file_content, idx );
		if ( idx == file_content.size() ) {
			break;
		}
		constant endline_idx = ate_chars_until( file_content, idx, '\n' );
		CString temp{ file_content.data() + idx, endline_idx - idx };

		if ( temp.begins_with( comment_mark ) ) {
			idx = endline_idx;
			continue;
		} else if ( temp.begins_with( section_mark ) ) {
			constant section_name_begin = idx + section_mark.size;
			constant section_name_end = ate_chars_until_whitespace( file_content, section_name_begin );
			current_section_hash = hash_cstring( { file_content.data() + section_name_begin, section_name_end - section_name_begin } );
			idx = endline_idx;
			continue;
		}

		constant name_idx_begin = idx;
		constant name_idx_end = ate_chars_until_whitespace( file_content, idx );
		CString const name{ file_content.data() + name_idx_begin, name_idx_end - name_idx_begin };
		constant name_hash = hash_cstring( name );
		constant value_idx = ate_whitespace( file_content, name_idx_end );
		if ( value_idx >= endline_idx ) {
			con_log_indented( 1, R"(Warning: in file "%" field "%" has no value assigned!)", path, name );
			idx = endline_idx;
			++current_config_value; // We're wasting space here but no value is a bug either way so it's not bad. 
			continue;
		}
		constant value_str_size = ( ate_whitespace_reversed( file_content, endline_idx - 1 ) + 1 ) - value_idx;

		char* value_buffer = reinterpret_cast<char*>( Context.default_allocator->allocate( value_str_size ) );
		memcpy( value_buffer, file_content.data() + value_idx, value_str_size );

		CString const value{ value_buffer, value_str_size };

		config_values[current_config_value] ={
			.section_hash = current_section_hash,
			.name_hash = name_hash,
			.value = value
		};
		idx = endline_idx;

		++current_config_value;
		if ( current_config_value >= config_values.size() ) {
			con_log_indented( 1, R"(Error: our limit for config values, %, has been exceeded! File: "%".)", CON_MAX_CONFIG_ENTRIES, path );
			return false;
		}
	}

	config_values.shrink( current_config_value );

	return true;
}

returning Config_File::parse_from_source( CString source ) -> bool
{
	auto temporary_allocator = reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );
	constant mark = temporary_allocator->get_mark();
	defer{ temporary_allocator->set_mark( mark ); };

	// @Robustness: We can avoid this by having methods that accept CString instead of Array<char>.
	// This will do it for now tho.
	Array<char> source_content;
	source_content.initialize( source.size, temporary_allocator );
	memcpy( source_content.data(), source.data, source.size );

	config_values.initialize( CON_MAX_CONFIG_ENTRIES, Context.default_allocator );

	u32 current_section_hash = 0;
	// Count lines which aren't empty, aren't a comment and aren't a section beginning.
	// We use this value to determine how much memory we need for `config_values`.
	s32 current_config_value = 0;

	for ( s32 idx = 0; idx < source_content.size(); ) {
		idx = ate_whitespace( source_content, idx );
		if ( idx == source_content.size() ) {
			break;
		}
		constant endline_idx = ate_chars_until( source_content, idx, '\n' );
		CString temp{ source_content.data() + idx, endline_idx - idx };

		if ( temp.begins_with( comment_mark ) ) {
			idx = endline_idx;
			continue;
		} else if ( temp.begins_with( section_mark ) ) {
			constant section_name_begin = idx + section_mark.size;
			constant section_name_end = ate_chars_until_whitespace( source_content, section_name_begin );
			current_section_hash = hash_cstring( { source_content.data() + section_name_begin, section_name_end - section_name_begin } );
			idx = endline_idx;
			continue;
		}

		constant name_idx_begin = idx;
		constant name_idx_end = ate_chars_until_whitespace( source_content, idx );
		CString const name{ source_content.data() + name_idx_begin, name_idx_end - name_idx_begin };
		constant name_hash = hash_cstring( name );
		constant value_idx = ate_whitespace( source_content, name_idx_end );
		if ( value_idx >= endline_idx ) {
			con_log_indented( 1, R"(Warning: in DEFAULT CONFIG field "%" has no value assigned!)", name );
			idx = endline_idx;
			++current_config_value; // We're wasting space here but no value is a bug either way so it's not bad. 
			continue;
		}
		constant value_str_size = ( ate_whitespace_reversed( source_content, endline_idx - 1 ) + 1 ) - value_idx;

		char* value_buffer = reinterpret_cast<char*>( Context.default_allocator->allocate( value_str_size ) );
		memcpy( value_buffer, source_content.data() + value_idx, value_str_size );

		CString const value{ value_buffer, value_str_size };

		config_values[current_config_value] ={
			.section_hash = current_section_hash,
			.name_hash = name_hash,
			.value = value
		};
		idx = endline_idx;

		++current_config_value;
		if ( current_config_value >= config_values.size() ) {
			con_log_indented( 1, "(Error: our limit for config values, %, has been exceeded! (in DEFAULT CONFIG)", CON_MAX_CONFIG_ENTRIES );
			return false;
		}
	}

	config_values.shrink( current_config_value );

	return true;
}

void Config_File::free()
{
	for ( s32 i = 0; i < config_values.size(); ++i ) {
		Context.default_allocator->free( reinterpret_cast<byte*>( const_cast<char*>( config_values[i].value.data ) ), config_values[i].value.size );
	}

	config_values.shutdown();
}

returning Config_File::get_value( Hashed_CString section, Hashed_CString name ) -> CString
{
	for ( s32 i = 0; i < config_values.size(); ++i ) {
		auto& info = config_values[i];
		if ( info.section_hash == section.hash &&
			 info.name_hash == name.hash ) {
			return info.value;
		}
	}

	return {};
}

returning Config_File::get_section( Hashed_CString section ) -> Array<Hash_Value_Pair>
{
	// @Robustness: Another use-case for a growing array.

	// First, we have to count how much elements requested section has.
	s32 entries_count = 0;
	for ( s32 i = 0; i < config_values.size(); ++i ) {
		if ( config_values[i].section_hash == section.hash ) {
			++entries_count;
		}
	}

	con_assert( entries_count > 0 );
	if ( entries_count == 0 ) {
		return {};
	}

	// Now we're actually gathering them
	Array<Hash_Value_Pair> to_return;
	to_return.initialize( entries_count, Context.temporary_allocator );

	s32 current_entry = 0;
	for ( s32 i = 0; i < config_values.size() && current_entry < entries_count; ++i ) {
		constant& cfg_val = config_values[i];
		if ( cfg_val.section_hash == section.hash ) {
			auto& entry = to_return[current_entry];

			entry.hash = cfg_val.name_hash;
			entry.value = cfg_val.value;
			++current_entry;
		}
	}

	con_assert( current_entry == entries_count );
	return to_return;
}
}