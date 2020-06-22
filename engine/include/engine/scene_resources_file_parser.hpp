#pragma once

#include <engine/array.hpp>
#include <engine/cstring.hpp>

namespace con
{
returning parse_scene_resources_file( CString path, Array<CString>& textures, Array<CString>& fonts, Array<CString>& shaders ) -> bool;
}