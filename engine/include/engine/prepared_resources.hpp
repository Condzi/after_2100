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
	returning get_starting_planet_hash() const -> u32;

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

	// @Robustness: is it really the place for starting_planet_hash?
	// If you gonna refactor this, see the prepare_resources_for_scene, this is
	// where we set it at the time of writing this note.
	u32 starting_planet_hash = 0;
};
}