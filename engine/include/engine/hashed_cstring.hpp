#pragma once

#include <engine/cstring.hpp>

namespace con
{
struct Hashed_CString final
{
	u32 hash = 0;

	constexpr Hashed_CString( CString str_to_hash ) :
		hash( hash_cstring( str_to_hash ) )
	{}
};

static Hashed_CString operator "" _hcs( char const* const str, size_t size )
{
	return { CString( str, static_cast<s32>( size ) ) };
}
}