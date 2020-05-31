#include <engine/config_file.hpp>
#include <engine/allocator.hpp>

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

void Config_File::parse( CString path )
{
	con_assert( config_values.size() <= 0 ); // only one config file per parse for now.

	auto temporary_allocator = reinterpret_cast<Temporary_Allocator*>( Context.temporary_storage_allocator );
	constant mark = temporary_allocator->get_mark();
	defer{ temporary_allocator->set_mark( mark ); };

	std::ifstream input( std::string{ path.data, path.data + path.size }, std::ios::binary );
	release_con_assert( input.is_open() );

	constant file_size = static_cast<s32>( std::filesystem::file_size( { path.data, path.data + path.size } ) );

	Array<char> file_content;
	file_content.initialize( file_size, temporary_allocator );
	// @Robustness: Is this correct size?
	input.read( file_content.data(), file_size );
	input.close();

	// Count lines which aren't empty, aren't a comment and aren't a section beginning.
	// We use this value to determine how much memory we need for `config_values`
	s32 config_values_count = 0;

	for ( s32 idx = 0; idx < file_content.size(); idx = ate_chars_until( file_content, idx, '\n' ) ) {
		idx = ate_whitespace( file_content, idx );
		CString temp{ file_content.data() + idx, file_content.size() - idx };

		if ( !temp.begins_with( section_mark ) &&
			 !temp.begins_with( comment_mark ) ) {
			++config_values_count;
		}
	}

	config_values.initialize( config_values_count, Context.default_allocator );

	u32 current_section_hash = 0;
	s32 current_config_value = 0;
	for ( s32 idx = 0; idx < file_content.size(); ) {
		idx = ate_whitespace( file_content, idx );
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
		CString name{ file_content.data() + name_idx_begin, name_idx_end - name_idx_begin };
		constant name_hash = hash_cstring( name );
		constant value_idx = ate_whitespace( file_content, name_idx_end );
		constant value_str_size = ( ate_whitespace_reversed(file_content, endline_idx) + 1 ) - value_idx;
		CString value{ reinterpret_cast<char*>( Context.default_allocator->allocate( value_str_size ) ), value_str_size };
		memcpy( const_cast<char*>( value.data ), file_content.data() + value_idx, value_str_size );

		config_values[current_config_value] ={
			.section_hash = current_section_hash,
			.name_hash = name_hash,
			.value = value
		};
		++current_config_value;
		idx = endline_idx;
	}

	con_assert( config_values_count == current_config_value );
}

Config_File::~Config_File()
{
	for ( s32 i = 0; i < config_values.size(); ++i ) {
		Context.default_allocator->free( reinterpret_cast<byte*>( const_cast<char*>( config_values[i].value.data ) ), config_values[i].value.size );
	}

	config_values.shutdown();
}

returning Config_File::get_value( CString section, CString name ) -> CString
{
	constant section_hash = hash_cstring( section );
	constant name_hash    = hash_cstring( name );

	for ( s32 i = 0; i < config_values.size(); ++i ) {
		auto& info = config_values[i];
		if ( info.section_hash == section_hash &&
			 info.name_hash == name_hash ) {
			return info.value;
		}
	}

	return {};
}
}