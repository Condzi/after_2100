#include <engine/utility.hpp>
#include <engine/logger.hpp>

#include <filesystem>
#include <fstream>

namespace con
{
returning load_entire_file_binary( CString path, Allocator* allocator ) -> File_Loading_Result
{
	namespace fs = std::filesystem;

	constant path_sv = cstring_to_stdsv( path );

	std::error_code fs_error_code;
	constant file_size = static_cast<s32>( fs::file_size( path_sv, fs_error_code ) );

	if ( fs_error_code ) {
		con_log( R"(Error: can't read file size for "%". Info: "%".)", path, cstring_from_stdstring( fs_error_code.message() ) );
		return { .success = false };
	}

	std::ifstream input( path_sv, std::ios::binary );
	if ( input.is_open() == false ) {
		con_log( R"(Error: couldn't open file "%".)", path );
		return { .success = false };
	}

	Array<char> file_content;
	file_content.initialize( file_size, allocator );
	input.read( file_content.data(), file_size );
	input.close();

	return { .content = file_content, .success = true };
}

returning ate_chars_until( Array<char> const& arr, s32 current_idx, char c ) -> s32
{
	s32 idx = current_idx;
	for ( ; idx != arr.size() && arr[idx] != c; ++idx );
	return idx;
}

returning ate_chars_until_whitespace( Array<char> const& arr, s32 current_idx ) -> s32
{
	s32 idx = current_idx;
	for ( ; idx != arr.size() && ::isspace( arr[idx] ) == 0; ++idx );
	return idx;
}

returning ate_whitespace_reversed( Array<char> const& arr, s32 current_idx ) -> s32
{
	s32 idx = current_idx;
	for ( ; idx != 0 && ::isspace( arr[idx] ) != 0; --idx );
	return idx;
}

returning ate_whitespace( Array<char> const& arr, s32 current_idx ) -> s32
{
	s32 idx = current_idx;
	for ( ; idx != arr.size() && ::isspace( arr[idx] ) != 0; ++idx );
	return idx;
}
}
