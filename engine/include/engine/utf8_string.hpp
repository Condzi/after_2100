#pragma once

#include <engine/typedefs.hpp>

namespace con
{
// Works like a CString. Doesn't have it's functions, tho.
// We use it only to represent text on the screen.
struct UTF8_String final
{
	wchar_t const* const data = nullptr;
	s32 const            length = 0;

	UTF8_String() = default;
	UTF8_String( wchar_t const* const runtime_str, s32 length_ );
	template <s32 TLength>
	constexpr UTF8_String( wchar_t const ( &cstr )[TLength] );

	UTF8_String& operator=( UTF8_String const& other );
};

//
// Definitions
//

template <s32 TLength>
constexpr UTF8_String::UTF8_String( wchar_t const( &cstr )[TLength] ) :
	data( cstr ),
	length( TLength-1 )
{}

inline UTF8_String operator "" _utf8( wchar_t const* const str, size_t size )
{
	return { str, static_cast<s32>( size ) };
}
}