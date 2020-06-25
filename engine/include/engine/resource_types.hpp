#pragma once

#include <engine/typedefs.hpp>

namespace con
{
struct Texture final
{
	u32 name_hash = 0;
	gl_id id = 0;
};

struct Shader final
{
	u32 name_hash = 0;
	gl_id id = 0;
};

// Note: Should be returned as const, we don't want to change it in any way after loading.
struct Texture_Data final
{
	s16 width = -1, height = -1;
	// Texture_Wraping_Mode wraping_mode; // one of the GL_TEXTURE_REPEAT etc
	// Maybe a way to get the raw pixels data?
};

}