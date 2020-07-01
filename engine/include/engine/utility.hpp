#pragma once

#include <engine/array.hpp>
#include <engine/cstring.hpp>

namespace con
{
//
// File parsing utilities.
//

struct File_Loading_Result final
{
	Array<char> content;
	bool success = true;
};

// Allocates using temporary allocator. Returns Array<char> instead of <bytes> for
// convenience.
returning load_entire_file_binary( CString path ) -> File_Loading_Result;

returning ate_chars_until( Array<char> const& arr, s32 current_idx, char c ) -> s32;
returning ate_chars_until_whitespace( Array<char> const& arr, s32 current_idx ) -> s32;
returning ate_whitespace_reversed( Array<char> const& arr, s32 current_idx ) -> s32;
returning ate_whitespace( Array<char> const& arr, s32 current_idx ) -> s32;

}