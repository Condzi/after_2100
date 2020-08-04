#pragma once

#include <engine/array.hpp>
#include <engine/cstring.hpp>
#include <engine/resource_types.hpp>

namespace con
{

class Resource_Loader final
{
public:
	// Works like reload but doesn't attempt to update entities.
	// Just loads the metadata and default resources.
	// Loads also planets.variables
	void initialize();
	void shutdown();

	// We're not reloading local.variables file!
	// We're reloading:
	// -- .scene_resources file (and default.scene_resources)
	// -- assets.variables
	// Then we're updating currently used resources (listed in current & default
	// scene_resources file).
	void reload();

	returning prepare_resources_for_scene( CString scene_name ) -> bool;

private:
	struct
	{
		Array<u32> textures;
		Array<u32> shaders;

		Array<u32> planets;
	} name_hashes;

	// Allocated using Default Allocator!
	struct
	{
		Array<CString> textures;
		Array<CString> shaders;
	} paths;

	struct
	{
		Array<Texture> textures;
		Array<Shader> shaders;
	} defaults; // don't unload these!

	// @Robustness: right now the fallback resources are reduntant in prepared_resources.
	// Decide where they should be stored -- in resource_loader or in prepared_resources?
	struct
	{
		Texture texture;
		Shader shader;
	} fallback;

	struct
	{
		// We check against this when preparing resources for a scene.
		Array<u32> hashes;
	} scene_folder_content;

	// (idx associated with `name_hashes`)
	Array<Texture_Data> texture_data;
	Array<Planet_Resource_Data> planet_data;

	// @Robustness: Add Bitset that holds N values, one for each resource of each type,
	// that indicates if given resource was correctly loaded. This way we could have
	// better error messages (I can't do it now because I didn't fix the allocator
	// handling in the Bitset class)

	void check_scene_folder_content();

	returning init_texture( byte const* data, s16 width, s16 height ) -> gl_id;
};
}