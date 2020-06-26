#pragma once

#include <engine/array.hpp>
#include <engine/cstring.hpp>

namespace con
{
returning parse_scene_resources_file( CString path, Array<u32>& textures, Array<u32>& fonts, Array<u32>& shaders ) -> bool;
}