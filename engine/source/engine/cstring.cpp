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

// Implementation of the Fowler–Noll–Vo-1a hash function
// Source: https://www.gamedev.net/forums/topic/634740-help-with-string-hashing/5004080/?page=1
returning hash_cstring( CString cstring ) -> u32
{
	u32 hash = 0x811C9DC5; // Actually I use a defined constant here, but this is the seed for a 32-bit hash
	for ( s32 i = 0; i < cstring.size; ++i ) {
		// The FNV-1a variation
		hash ^= cstring.data[i];
		hash *= 0x01000193; // Same for this, this is the 32-bit prime number
	}
	return hash;
}

returning cstring_from_cstr( char const* cstr ) -> CString
{
	return { cstr, static_cast<s32>( strlen( cstr ) ) };
}
}