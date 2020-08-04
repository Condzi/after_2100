#pragma once

#include <engine/array.hpp>
#include <engine/hashed_cstring.hpp>
#include <engine/resource_types.hpp>
#include <engine/font.hpp>

#include <glad/glad.h>

namespace con
{
class Prepared_Resources final
{
	friend class Resource_Loader;

public:
	returning get_texture( Hashed_CString name ) -> Texture;
	returning get_shader( Hashed_CString name ) -> Shader;
	returning get_font( Hashed_CString name ) -> Font&;
	returning get_starting_planet_hash() const -> u32;

	returning textures_count() const -> s32;
	returning shaders_count() const -> s32;

private:
	// @Important: We don't call shutdown for this arrays!!!
	// We should do it either in the Resource_Loader or here.
	// We're also not calling shutdown for Font.


	// We're assuming that first N resources are the default ones (which don't change)
	Array<Texture> textures;
	Array<Shader>  shaders;

	// Font class is kinda big. It holds the font file, which may be even 1MB in size.
	// Because of that, I decided to split the name hash and actual font to two arrays
	// for quicker lookup.
	Array<u32>  fonts_names_hashes;
	Array<Font> fonts;

	Texture fallback_texture;
	Shader  fallback_shader;
	// @ToDo: add fallback font!
	Font    fallback_font; // this font should be located in program memory. Find some nice, small font.

	// @Robustness: is it really the place for starting_planet_hash?
	// If you gonna refactor this, see the prepare_resources_for_scene, this is
	// where we set it at the time of writing this note.
	u32 starting_planet_hash = 0;
};
}