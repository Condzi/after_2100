#pragma once

#include <engine/array.hpp>
#include <engine/hashed_cstring.hpp>
#include <engine/resource_types.hpp>

#include <glad/glad.h>

namespace con
{
class Prepared_Resources final
{
	friend class Resource_Loader;

public:
	returning get_texture( Hashed_CString name ) -> Texture;
	returning get_shader( Hashed_CString name ) -> Shader;

	returning textures_count() const -> s32;
	returning shaders_count() const -> s32;

private:
	// @Peroformance: For bigger structures we may want to separate the 
	// hash and use associative tables for lookup. 
	// We're assuming that first N resources are the default ones (which don't change)
	Array<Texture> textures;
	Array<Shader> shaders;

	Texture fallback_texture;
	Shader  fallback_shader;
};
}