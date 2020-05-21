#include <engine/sprint.hpp>

#include <charconv>

file_scope
{
	compile_constant conversion_buffer_size = 32;
	static char conversion_buffer[conversion_buffer_size] = { 0 };
}

namespace con
{

returning s32_to_cstring( s32 value ) -> CString
{
	auto [ptr, errc] = std::to_chars( conversion_buffer, conversion_buffer + conversion_buffer_size, value );

	conassert( errc == std::errc{} );

	constant string_size = static_cast<s32>( ptr - conversion_buffer );
	char* string_data = reinterpret_cast<char*>( Context.temporary_storage_allocator->allocate( string_size ) );

	return { string_data, string_size };
}

returning f32_to_cstring( f32 value ) -> CString
{
	auto [ptr, errc] = std::to_chars( conversion_buffer, conversion_buffer + conversion_buffer_size, value );

	conassert( errc == std::errc{} );

	constant string_size = static_cast<s32>( ptr - conversion_buffer );
	char* string_data = reinterpret_cast<char*>( Context.temporary_storage_allocator->allocate( string_size ) );

	std::memcpy( string_data, conversion_buffer, string_size );

	return { string_data, string_size };
}
}