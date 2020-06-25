#pragma once

#include <engine/array.hpp>
#include <engine/hashed_cstring.hpp>
#include <engine/typedefs.hpp>

#include <glad/glad.h>

namespace con
{
struct Texture final
{
	u32 name_hash = 0;
	gl_id id = 0;
};

// Not used in Prepared_Textures. But maybe should've been used because
// we may care about it? Instead of calling Resource_Manager we just simply
// ask the Prepared_Resources object for it. It may have an array of pointers
// to that data. Hence Resource_Manager would be just a big pile of paths and
// corresponding hashes of names, and Prepared_Resources would just have resources?
// @Robustness
// Note: Should be returned as const, we don't want to change it in any way after loading.
struct Texture_Data final
{
	s16 width = -1, heigt = -1;
	// Texture_Wraping_Mode wraping_mode; // one of the GL_TEXTURE_REPEAT etc
	// Maybe a way to get the raw pixels data?
};

struct Shader final
{
	u32 name_hash = 0;
	gl_id id = 0;
};

class Prepared_Resources final
{
	friend class Resource_Manager;

public:
	returning get_texture( Hashed_CString name ) -> Texture;
	returning get_shader( Hashed_CString name ) -> Shader;

	returning textures_count() const -> s32;
	returning shaders_count() const -> s32;

private:
	// @Peroformance: For bigger structures we may want to separate the 
	// hash and use associative tables for lookup. 
	Array<Texture> textures;
	Array<Shader> shaders;
};
}