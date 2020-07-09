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
	compile_constant fonts_section_hash    = hash_cstring( "fonts" );
	compile_constant shaders_section_hash  = hash_cstring( "shaders" );

	// hvp = hash_value_pair, we're getting array of that from cfg to copy 'hash' values
	constant textures_hvp = cfg.get_section( textures_section_hash );
	constant fonts_hvp    = cfg.get_section( fonts_section_hash );
	constant shaders_hvp  = cfg.get_section( shaders_section_hash );

	constant textures_count = textures_hvp.size();
	constant fonts_count    = fonts_hvp.size();
	constant shaders_count  = shaders_hvp.size();

	// Print 0 or count value.
	con_log_indented( 2, "Entries found: % textures, % fonts, % shaders.",
					  textures_count < 0 ? 0 : textures_count,
					  fonts_count    < 0 ? 0 : fonts_count,
					  shaders_count  < 0 ? 0 : shaders_count );

	if ( textures_count > 0 ) {
		file_content.textures.initialize( textures_count, Context.default_allocator );
	}
	if ( fonts_count > 0 ) {
		file_content.fonts.initialize( fonts_count, Context.default_allocator );
	}
	if ( shaders_count > 0 ) {
		file_content.shaders.initialize( shaders_count, Context.default_allocator );
	}

	//
	// For textures, fonts and shaders we only need to copy the name hashes.
	//

	for ( s32 i = 0; i < textures_count; ++i ) {
		file_content.textures[i] = textures_hvp[i].hash;
	}

	for ( s32 i = 0; i < fonts_count; ++i ) {
		file_content.fonts[i] = fonts_hvp[i].hash;
	}

	for ( s32 i = 0; i < shaders_count; ++i ) {
		file_content.shaders[i] = shaders_hvp[i].hash;
	}

	return { .success = true, .content = file_content };
}
}