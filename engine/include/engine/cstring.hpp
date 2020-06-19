#pragma once

#include <engine/typedefs.hpp>

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

returning operator==( CString const& lhs, CString const rhs ) -> bool;
returning hash_cstring( CString cstring ) -> u32;
// Stupid name tbh. Makes CString from C-string at runtime.
returning cstring_from_cstr( char const* cstr ) -> CString;

//
// Definitions
//

template <s32 SIZE>
constexpr CString::CString( char const( &cstr )[SIZE] ) :
	data( cstr ),
	size( SIZE-1 )
{}

static CString operator "" _cs( char const * const str, size_t size ) {
	return CString( str, static_cast<s32>( size ) );
}
}