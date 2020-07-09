#pragma once

#include <engine/array.hpp>
#include <engine/cstring.hpp>

namespace con
{
struct Scene_Resources_File_Content final
{
	Array<u32> textures;
	Array<u32> fonts;
	Array<u32> shaders;

	u32 starting_planet_hash = 0;
	// planets and planets_positions share indexes.
	Array<u32> planets;
	Array<v2> planets_positions;
};

struct Scene_Resources_File_Parsing_Data final
{
	bool success = false;
	Scene_Resources_File_Content content;
};

returning parse_scene_resources_file( CString path ) -> Scene_Resources_File_Parsing_Data;
}