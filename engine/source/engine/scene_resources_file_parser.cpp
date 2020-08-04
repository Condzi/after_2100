#include <engine/scene_resources_file_parser.hpp>

#include <engine/algorithms.hpp>
#include <engine/logger.hpp>
#include <engine/config_file.hpp>

#include <string>

namespace con
{
returning parse_scene_resources_file( CString path ) -> Scene_Resources_File_Parsing_Data
{
	auto temporary_allocator = reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );
	constant mark = temporary_allocator->get_mark();
	defer{ temporary_allocator->set_mark( mark ); };

	Config_File cfg;
	if ( !cfg.parse_from_file( path ) ) {
		return { .success = false };
	}

	Scene_Resources_File_Content file_content;

	//
	// Get sections and initialize the arguments with correct sizes.
	//

	compile_constant textures_section_hash = hash_cstring( "textures" );
	compile_constant shaders_section_hash  = hash_cstring( "shaders" );
	compile_constant planets_section_hash  = hash_cstring( "planets" );

	// hvp = hash_value_pair, we're getting array of that from cfg to copy 'hash' values
	constant textures_hvp = cfg.get_section( textures_section_hash );
	constant shaders_hvp  = cfg.get_section( shaders_section_hash );
	constant planets_hvp  = cfg.get_section( planets_section_hash );

	constant textures_count = textures_hvp.size();
	constant shaders_count  = shaders_hvp.size();
	constant planets_count  = planets_hvp.size() - 1; // Don't count starting_planet field

	// Print 0 or count value.
	con_log_indented( 2, "Entries found: % textures, % shaders, % planets.",
					  textures_count < 0 ? 0 : textures_count,
					  shaders_count  < 0 ? 0 : shaders_count,
					  planets_count  < 0 ? 0 : planets_count
	);
	//
	// Handle textures and shaders.
	//

	if ( textures_count > 0 ) {
		file_content.textures.initialize( textures_count, Context.default_allocator );
	}
	if ( shaders_count > 0 ) {
		file_content.shaders.initialize( shaders_count, Context.default_allocator );
	}

	//
	// For textures and shaders we only need to copy the name hashes.
	//

	for ( s32 i = 0; i < textures_count; ++i ) {
		file_content.textures[i] = textures_hvp[i].hash;
	}

	for ( s32 i = 0; i < shaders_count; ++i ) {
		file_content.shaders[i] = shaders_hvp[i].hash;
	}

	//
	// Handling planets info. One of which is starting_planet, so we don't count it in the array.
	//

	// We have to have at least one planet. (planets_count doesn't include starting_planet field)
	if ( planets_count > 0 ) {
		constant find_starting_planet_result = linear_find_if( planets_hvp, []( Config_File::Hash_Value_Pair const& hvp ) {
			return hvp.hash == hash_cstring( "starting_planet" );
		} );

		if ( find_starting_planet_result.found() ) {
			constant idx = find_starting_planet_result.idx;
			file_content.starting_planet_hash = hash_cstring( planets_hvp[idx].value );
			con_log_indented( 2, "Starting planet is \"%\".", planets_hvp[idx].value );
		} else {
			con_log_indented( 2, "Error: no info about starting planet. Assigning first planet in the array... " );
			if ( planets_count > 0 ) {
				con_log_indented( 3, "Setting starting_planet_hash to %.", planets_hvp[0].hash );

				file_content.starting_planet_hash = planets_hvp[0].hash;
			} else {
				con_log_indented( 3, "Error: no planets! Setting starting_planet_hash to 0." );
				file_content.starting_planet_hash = 0;
			}
		}

		constant starting_planet_idx = find_starting_planet_result.idx;
		file_content.planets.initialize( planets_count, Context.default_allocator );
		file_content.planets_positions.initialize( planets_count, Context.default_allocator );


		s32 planets_idx = 0;
		for ( s32 i = 0; i < planets_hvp.size(); ++i ) {
			if ( i == starting_planet_idx ) {
				continue;
			}

			file_content.planets[planets_idx] = planets_hvp[i].hash;

			v2 position;
			constant parsing_result = sscan( "% %", planets_hvp[i].value, position.x, position.y );
			if ( !parsing_result ) {
				con_log_indented( 2, "Error: failed to parse planet position! (hash: %, value: %", planets_hvp[i].hash, planets_hvp[i].value );

				file_content.planets_positions[planets_idx] = v2{ 0,0 };
			} else {
				file_content.planets_positions[planets_idx] = position;
			}

			++planets_idx;
		}
	}

	return { .success = true, .content = file_content };
}
}