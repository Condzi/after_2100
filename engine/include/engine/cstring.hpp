#pragma once

#include <engine/array.hpp>
#include <string>

namespace con
{
struct CString final
{
	// Holds 'size' characters + \0 (but we don't include NULL in our size variable)
	char const* const data = nullptr;
	// @Robustness: We don't really have that log strings. Maybe we can use s16 here?
	s32 const size = 0;

	CString() = default;
	CString( char const* const runtime_str, s32 size_ );
	template <s32 SIZE>
	constexpr CString( char const ( &cstr )[SIZE] );

	returning begins_with( CString str ) const -> bool;

	CString& operator=( CString const& other );
};

//
// Declarations
//

returning operator==( CString const& lhs, CString const rhs ) -> bool;
returning constexpr hash_cstring( CString const cstring ) -> u32;

// Stupid name tbh. Makes CString from C-string at runtime.
// @Robustness: should we create these in temporary buffer?
returning cstring_from_cstr( char const* cstr ) -> CString;
returning cstring_from_stdstring( std::string const& str ) -> CString;
returning cstring_from_array( Array<char> const& arr ) -> CString;

// Allocates in temporary storage. Just adds \0 to the end
returning cstring_to_cstr( CString str ) -> CString;
returning cstring_to_stdsv( CString str ) -> std::string_view;

//
// Definitions
//

template <s32 SIZE>
constexpr CString::CString( char const( &cstr )[SIZE] ) :
	data( cstr ),
	size( SIZE-1 )
{}

inline CString operator "" _cs( char const* const str, size_t size )
{
	return CString( str, static_cast<s32>( size ) );
}

returning constexpr hash_cstring( CString const cstring ) -> u32
{
	// Implementation of the Fowler–Noll–Vo-1a hash function
	// Source: https://www.gamedev.net/forums/topic/634740-help-with-string-hashing/5004080/?page=1

	u32 hash = 0x811C9DC5; // Actually I use a defined constant here, but this is the seed for a 32-bit hash
	for ( s32 i = 0; i < cstring.size; ++i ) {
		// The FNV-1a variation
		hash ^= cstring.data[i];
		hash *= 0x01000193; // Same for this, this is the 32-bit prime number
	}
	return hash;
}
}