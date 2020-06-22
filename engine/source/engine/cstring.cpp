#include <engine/cstring.hpp>

#include <cstring>

namespace con
{
CString::CString( char const* const runtime_str, s32 size_ ) :
	data( runtime_str ),
	size( size_ )
{}

returning CString::begins_with( CString str ) const -> bool
{
	return size >= str.size && memcmp( data, str.data, str.size ) == 0;
}

CString& CString::operator=( CString const& other )
{
	std::memcpy( this, &other, sizeof( CString ) );

	return *this;
}

returning operator==( CString const& lhs, CString const rhs ) -> bool
{
	return ( lhs.size == rhs.size ) && ( lhs.data == rhs.data || memcmp( lhs.data, rhs.data, lhs.size ) == 0 );
}

returning cstring_from_cstr( char const* cstr ) -> CString
{
	return { cstr, static_cast<s32>( strlen( cstr ) ) };
}
returning cstring_from_stdstring( std::string const& str ) -> CString
{
	return { str.data(), static_cast<s32>( str.size() ) };
}
}