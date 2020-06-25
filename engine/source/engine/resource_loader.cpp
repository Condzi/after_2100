#include <engine/resource_loader.hpp>
#include <engine/allocator.hpp>
#include <engine/logger.hpp>
#include <engine/config_file.hpp>
#include <engine/scene_resources_file_parser.hpp>

#include <glad/glad.h>

#include <filesystem>
//
// Defining own STB_Image stuff. We use temporary storage for fast allocations, we
// don't need images to live long anyway.
//
#define STBI_ONLY_PNG
#define STBI_ASSERT( x ) con_assert( x )
#define STBI_MALLOC( x ) Context.temporary_allocator->allocate( x )
#define STBI_REALLOC_SIZED( ptr, old_size, new_size ) \
{ \
con::byte* new_ptr_ = Context.temporary_allocator->allocate( new_size ); \
memcpy( new_ptr_, ptr, old_size ); \
ptr = reinterpret_cast<decltype( ptr )>( new_ptr_ ); \
}
#define STBI_FREE( x ) // we're using temporary storage so we're not freeing anything

#include <stb/stb_image.h>

namespace con
{
file_scope
{
returning load_texture_data( CString path, s16 const decl_width, s16 const decl_height ) -> byte*
{
	path = cstring_to_cstr( path );

	s32 loaded_width = -1, loaded_height = -1;
	s32 channels = -1;
	byte* data = reinterpret_cast<byte*>( stbi_load( path.data, &loaded_width, &loaded_height, &channels, 0 ) );

	// idk if we actually have to remove \0 so I do this just in case it messes
	// up logger / fputs
	path = { path.data, path.size-1 };

	if ( data == nullptr ) {
		constant failure_reason = cstring_from_cstr( stbi_failure_reason() );
		con_log_indented( 3, R"(Error: can't load texture from "%". Reason: "%".)", path, failure_reason );
		return nullptr;
	}

	if ( decl_width != static_cast<s16>( loaded_width ) ) {
		con_log_indented( 3, R"(Error: I refuse to load a texture from "%" because declared width was %px but loaded is %px.)", path, decl_width, loaded_width );
		return nullptr;
	}

	if ( decl_height != static_cast<s16>( loaded_height ) ) {
		con_log_indented( 3, R"(Error: I refuse to load a texture from "%" because declared height was %px but loaded is %px.)", path, decl_height, loaded_height );
		return nullptr;
	}

	return data;
}
}

void Resource_Loader::initialize()
{
	check_scene_folder_content();

	//
	// Getting assets metadata 
	//
	con_log_indented( 1, "Loading resource metadata from \"%\"...", CString{ CON_ASSETS_CONFIG_FILE } );
	Config_File assets_config;
	assets_config.parse_from_file( CON_ASSETS_CONFIG_FILE );
	// Config_File::get_section returns temporary storage memory, so we can reset it after we
	// use the array.
	auto& da     = *reinterpret_cast<Default_Allocator*>( Context.default_allocator );
	auto& ta     = *reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );

	// Handling textures metadata.
	{
		auto ta_mark = ta.get_mark();
		constant textures_section = assets_config.get_section( "textures"_hcs );
		constant textures_count = textures_section.size();

		name_hashes.textures.shutdown();
		paths.textures.shutdown();
		texture_data.shutdown();
		name_hashes.textures.initialize( textures_count );
		paths.textures.initialize( textures_count );
		texture_data.initialize( textures_count );

		s32 current_texture_idx = 0;

		CString path;
		s32 width = -1, height = -1;
		if ( textures_section.size() < 0 ) {
			con_log_indented( 1, R"(Error: Can't find "textures" section! )" );
		} else {
			for ( s32 i = 0; i < textures_section.size(); ++i ) {
				constant& it = textures_section[i];
				name_hashes.textures[current_texture_idx] = it.hash;

				if ( !sscan( "% % %", it.value, path, width, height ) ) {
					con_log_indented( 1, R"(Error: can't format the texture value "%"!)", it.value );
				} else {
					char* path_memory = da.allocate<char>( path.size );
					memcpy( path_memory, path.data, path.size );
					paths.textures[current_texture_idx] ={ path_memory, path.size };
					texture_data[current_texture_idx] ={ static_cast<s16>( width ), static_cast<s16>( height ) };
					// (if you need to you can print texture info here)
				}
				++current_texture_idx;
			}
		}
		ta.set_mark( ta_mark );
	}

	// Handling shaders metadata.
	{
		auto ta_mark = ta.get_mark();
		constant shaders_section = assets_config.get_section( "shaders"_hcs );
		constant shaders_count = shaders_section.size();

		name_hashes.shaders.shutdown();
		paths.shaders.shutdown();
		name_hashes.shaders.initialize( shaders_count );
		paths.shaders.initialize( shaders_count );

		s32 current_shader_idx = 0;

		CString path;
		if ( shaders_section.size() < 0 ) {
			con_log_indented( 1, R"(Error: Can't find "shaders" section! )" );
		} else {
			for ( s32 i = 0; i < shaders_section.size(); ++i ) {
				constant& it = shaders_section[i];
				name_hashes.textures[current_shader_idx] = it.hash;

				if ( !sscan( "%", it.value, path ) ) {
					con_log_indented( 1, R"(Error: can't format the shader value "%"!)", it.value );
				} else {
					char* path_memory = da.allocate<char>( path.size );
					memcpy( path_memory, path.data, path.size );
					paths.shaders[current_shader_idx] ={ path_memory, path.size };
				}
				++current_shader_idx;
			}
		}
		ta.set_mark( ta_mark );
	}

	assets_config.free();
	con_log_indented( 1, "Metadata loaded." );
}

void Resource_Loader::check_scene_folder_content()
{
	namespace fs = std::filesystem;
	
	scene_folder_content.hashes.shutdown();

	// @Robustness!!!!!!
	// Yet another use case for growing array concept!
	// @Performance? -- we are doing the same operation of iterating over directory
	// entries twice.

	//
	// Gathering the files count
	//
	s32 files_count = 0;
	std::error_code ec;
	auto directory_iterator = fs::directory_iterator( CON_SCENES_FOLDER, ec );
	if ( ec ) {
		con_log_indented( 2, R"(Error: can't iterate over scenes folder ("%"). Reason: "%".)", CString{ CON_SCENES_FOLDER }, cstring_from_stdstring( ec.message() ) );
		return;
	}
	for ( auto& entry : directory_iterator ) {
		constant path = entry.path();
		if ( !path.has_extension() || !path.has_filename() ) {
			continue;
		}

		if ( strcmp( path.extension().generic_string().c_str(), CON_SCENE_RESOURCES_FILE_EXTENSION ) == 0 ) {
			++files_count;
		}
	}

	if ( files_count == 0 ) {
		con_log_indented( 2, "Warning: no scene files found." );
		return;
	} 
	con_log_indented( 2, R"(Found % "%" files.)", files_count, CString{ CON_SCENE_RESOURCES_FILE_EXTENSION } );

	scene_folder_content.hashes.initialize( files_count );

	//
	// Actually getting the files names and hashing them
	//
	s32 current_file = 0;
	for ( auto& entry : directory_iterator ) {
		constant path = entry.path();
		if ( !path.has_extension() || !path.has_filename() ) {
			continue;
		}

		if ( strcmp( path.extension().generic_string().c_str(), CON_SCENE_RESOURCES_FILE_EXTENSION ) == 0 ) {

			constant filename = cstring_from_stdstring( path.filename().generic_string() );

			scene_folder_content.hashes[current_file] = hash_cstring( filename );

			if ( ++current_file == files_count ) {
				break;
			}
		}
	}

	release_con_assert( current_file == files_count );
}
}