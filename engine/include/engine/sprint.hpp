#pragma once

#include <engine/cstring.hpp>
#include <engine/allocator.hpp>
#include <engine/context.hpp>

namespace con
{
returning s32_to_cstring( s32 value ) -> CString;
returning f32_to_cstring( f32 value ) -> CString;
}