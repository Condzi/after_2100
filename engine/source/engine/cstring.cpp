#include <engine/cstring.hpp>
#include <engine/allocator.hpp>

#include <engine/array.hpp>

#include <cstring>

namespace con
{
CString::CString( char const* const runtime_str, s32 length_ ) :
	data( runtime_str ),
	length( length_ )
{}

returning CString::begins_with( CString str ) const -> bool
{
	return length >= str.length && memcmp( data, str.data, str.length ) == 0;
}

CString& CString::operator=( CString const& other )
{
	memcpy( this, &other, sizeof( CString ) );

	return *this;
}

returning operator==( CString const& lhs, CString const rhs ) -> bool
{
	return ( lhs.length == rhs.length ) && ( lhs.data == rhs.data || memcmp( lhs.data, rhs.data, lhs.length ) == 0 );
}

returning cstring_from_cstr( char const* cstr ) -> CString
{
	return { cstr, static_cast<s32>( strlen( cstr ) ) };
}
returning cstring_from_stdstring( std::string const& str ) -> CString
{
	constant size = static_cast<s32>( str.size() );
	char* str_data = reinterpret_cast<char*>( Context.temporary_allocator->allocate( size ) );

	memcpy( str_data, str.c_str(), size );

	return { str_data, size };
}

returning cstring_from_array( Array<char> const& arr ) -> CString
{
	return CString{ arr.data(), arr.size() };
}

returning cstring_to_cstr( CString str ) -> CString
{
	char* new_str_data = reinterpret_cast<char*>( Context.temporary_allocator->allocate( str.length + 1 ) );
	memcpy( new_str_data, str.data, str.length );
	new_str_data[str.length] = '\0';

	return CString{ new_str_data, str.length+1 };

}

returning cstring_to_stdsv( CString str ) -> std::string_view
{
	return std::string_view{ str.data, static_cast<size_t>( str.length ) };
}

returning cstring_to_utf8_string( CString str ) -> UTF8_String
{
	auto& ta = reinterpret_cast<Temporary_Allocator&>( *Context.temporary_allocator );

	wchar_t* utf8_data = ta.allocate<wchar_t>( str.length );

	// Just copy chars values to utf8_data. We can't use memcpy
	// because wchar_t is bigger than a char.

	for ( s32 i = 0; i < str.length; ++i ) {
		utf8_data[i] = str.data[i];
	}

	return { utf8_data, str.length };
}
}