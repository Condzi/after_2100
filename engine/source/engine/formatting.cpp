#include <engine/formatting.hpp>

#include <charconv>

// We don't use con_assert because it uses formatting too.
#include <cassert>

file_scope
{
	compile_constant conversion_buffer_size = 32;
	static char conversion_buffer[conversion_buffer_size] = { 0 };
}

namespace con
{

returning bool_to_cstring( bool value ) -> CString
{
	if ( value ) {
		return "true"_cs;
	}

	return "false"_cs;
}

returning s32_to_cstring( s32 value ) -> CString
{
	auto [ptr, errc] = std::to_chars( conversion_buffer, conversion_buffer + conversion_buffer_size, value );

	con_assert( errc == std::errc{} );

	constant string_size = static_cast<s32>( ptr - conversion_buffer );
	char* string_data = reinterpret_cast<char*>( Context.temporary_allocator->allocate( string_size ) );

	std::memcpy( string_data, conversion_buffer, string_size );

	return { string_data, string_size };
}

returning f32_to_cstring( f32 value ) -> CString
{
	auto [ptr, errc] = std::to_chars( conversion_buffer, conversion_buffer + conversion_buffer_size, value );

	con_assert( errc == std::errc{} );

	constant string_size = static_cast<s32>( ptr - conversion_buffer );
	char* string_data = reinterpret_cast<char*>( Context.temporary_allocator->allocate( string_size ) );

	std::memcpy( string_data, conversion_buffer, string_size );

	return { string_data, string_size };
}

returning cstring_to_s32( CString str ) -> s32
{
	s32 converted_value = 0;
	constant errc = std::from_chars( str.data, str.data + str.length, converted_value ).ec;

	con_assert( errc == std::errc{} );

	return converted_value;
}

returning cstring_to_f32( CString str ) -> f32
{
	f32 converted_value = 0;
	constant errc = std::from_chars( str.data, str.data + str.length, converted_value ).ec;

	con_assert( errc == std::errc{} );

	return converted_value;
}

auto cstring_to_bool( CString str ) -> bool
{
	return str == "true"_cs || str == "TRUE"_cs;
}
}