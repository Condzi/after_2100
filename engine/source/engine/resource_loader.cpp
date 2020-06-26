#include <engine/resource_loader.hpp>
#include <engine/algorithms.hpp>
#include <engine/allocator.hpp>
#include <engine/logger.hpp>
#include <engine/config_file.hpp>
#include <engine/scene_resources_file_parser.hpp>
#include <engine/prepared_resources.hpp>

#include <glad/glad.h>

#include <fstream>
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
// 
// This fuctions allocate their data using temporary allocator!
//
file_scope
{
returning load_texture_data( CString path, s16 const decl_width, s16 const decl_height ) -> byte*
{
	path = cstring_to_cstr( path );

	s32 loaded_width = -1, loaded_height = -1;
	s32 channels = -1;
	byte* data = reinterpret_cast<byte*>( stbi_load( path.data, &loaded_width, &loaded_height, &channels, STBI_rgb_alpha ) );

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

returning load_shader_source( CString path ) -> CString
{
	std::ifstream input( cstring_to_stdsv( path ), std::ios::binary );
	if ( input.is_open() == false ) {
		con_log_indented( 1, R"(Error opening file "%".)", path );
		return {};
	}

	std::error_code fs_error_code;
	constant file_size = static_cast<s32>( std::filesystem::file_size( { path.data, path.data + path.size }, fs_error_code ) );
	if ( fs_error_code ) {
		con_log_indented( 1, R"(Error reading file size for "%", info: "%")", path, cstring_from_stdstring( fs_error_code.message() ) );
		return {};
	}

	Array<char> file_content;
	file_content.initialize( file_size, Context.temporary_allocator );
	input.read( file_content.data(), file_size );

	return cstring_from_array( file_content );
}

returning build_shader_program( CString source ) -> gl_id
{
	auto& ta = *reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );
	constant mark = ta.get_mark();
	defer{ ta.set_mark( mark ); };

	compile_constant vertex_shader_header   = CString{ "#version 330\n#define VERTEX_SHADER\n" };
	compile_constant fragment_shader_header = CString{ "#version 330\n#define FRAGMENT_SHADER\n" };

	// @ToDo: Check if we can remove the \0?
	constant vertex_shader_source   = sprint( "%%", vertex_shader_header, source );
	constant fragment_shader_source = sprint( "%%", fragment_shader_header, source );

	constant vertex_shader_id   = glCreateShader( GL_VERTEX_SHADER );
	constant fragment_shader_id = glCreateShader( GL_FRAGMENT_SHADER );
	constant program_id         = glCreateProgram();

	glShaderSource( vertex_shader_id, 1, &vertex_shader_source.data, &vertex_shader_source.size );
	glCompileShader( vertex_shader_id );

	glShaderSource( fragment_shader_id, 1, &fragment_shader_source.data, &vertex_shader_source.size );
	glCompileShader( fragment_shader_id );

	glAttachShader( program_id, vertex_shader_id );
	glAttachShader( program_id, fragment_shader_id );
	glLinkProgram( program_id );

	glDeleteShader( vertex_shader_id );
	glDeleteShader( fragment_shader_id );

	return program_id;
}
}

void Resource_Loader::initialize()
{
	stbi_set_flip_vertically_on_load( true ); // Because OpenGL... shrug

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

	//
	// Loading default resources info specified in default.scene_resources
	//

	con_log_indented( 1, R"(Loading default resources info from "%".)", CString{ CON_DEFAULT_SCENE_RESOURCES_INFO_FILE } );
	defaults.textures.shutdown();
	defaults.fonts.shutdown();
	defaults.shaders.shutdown();

	if ( !parse_scene_resources_file( CON_DEFAULT_SCENE_RESOURCES_INFO_FILE, defaults.textures, defaults.fonts, defaults.shaders ) ) {
		con_log_indented( 1, "Error: Loading failed!" );
	} else {
		con_log_indented( 1, "Loading succeed." );
	}

	//
	// Loading resources data and creating OpenGL stuff for them.
	//

	if ( defaults.textures.size() > 0 ) {
		con_log_indented( 1, "Loading default textures..." );

		auto& prepared_textures = Context.prepared_resources->textures;
		prepared_textures.initialize( defaults.textures.size() );

		for ( s32 i = 0; i < defaults.textures.size(); ++i ) {
			prepared_textures[i].name_hash = defaults.textures[i];

			constant[texture_width, texture_height] = texture_data[i];

			if ( auto data = load_texture_data( paths.textures[i], texture_width, texture_height );
				 data != nullptr ) {
				// @Performance: maybe we could call `glGenTextures` with an array of textures? 
				// I have to test it when we make this one work.
				gl_id texture_id = 0;
				glGenTextures( 1, &texture_id );
				glBindTexture( GL_TEXTURE_2D, texture_id );

				// set the texture wrapping parameters
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );	// set texture wrapping to GL_REPEAT (default wrapping method)
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
				// set texture filtering parameters
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
			}
		}
	} else {
		con_log_indented( 1, "No default textures to load." );
	}

	con_log_indented( 1, "Loading default fonts... (@INCOMPLETE, dummy)" );

	if ( defaults.shaders.size() > 0 ) {
		con_log_indented( 1, "Loading default shaders..." );

		auto& prepared_shaders = Context.prepared_resources->shaders;
		prepared_shaders.initialize( defaults.shaders.size() );

		for ( s32 i = 0; i < defaults.shaders.size(); ++i ) {
			prepared_shaders[i].name_hash = defaults.textures[i];

			constant source = load_shader_source( paths.shaders[i] );

			if ( source.size > 0 ) {
				constant shader_id = build_shader_program( source );
				prepared_shaders[i].id = shader_id;
			}
		}
	} else {
		con_log_indented( 1, "No default shaders to load." );
	}

}

void Resource_Loader::shutdown()
{
	name_hashes.textures.shutdown();
	name_hashes.shaders.shutdown();

	for ( s32 i = 0; i < paths.textures.size(); ++i ) {
		Context.default_allocator->free( reinterpret_cast<byte*>( const_cast<char*>( paths.textures[i].data ) ), paths.textures[i].size );
	}

	for ( s32 i = 0; i < paths.shaders.size(); ++i ) {
		Context.default_allocator->free( reinterpret_cast<byte*>( const_cast<char*>( paths.shaders[i].data ) ), paths.shaders[i].size );
	}

	paths.textures.shutdown();
	paths.shaders.shutdown();

	defaults.textures.shutdown();
	defaults.fonts.shutdown();
	defaults.shaders.shutdown();

	scene_folder_content.hashes.shutdown();

	texture_data.shutdown();
}

returning Resource_Loader::prepare_resources_for_scene( CString scene_name ) -> bool
{
	//
	// Check if this scene file exists (we could use the filesystem here too?)
	//
	constant scene_name_hash = hash_cstring( scene_name );

	constant result = linear_find( scene_folder_content.hashes, scene_name_hash );
	if ( !result.found() ) {
		con_log_indented( 1, R"(Error: can't find resource file for scene "%". Is name correct?)", scene_name );
		return false;
	}

	//
	// Get what resources we have to load. Skip the default ones.
	//
	constant scene_file_path = sprint( "%%%", CString{ CON_SCENES_FOLDER }, scene_name, CString{ CON_SCENE_RESOURCES_FILE_EXTENSION } );

	Array<u32> textures, fonts, shaders;

	if ( !parse_scene_resources_file( scene_file_path, textures, fonts, shaders ) ) {
		return false;
	}

	// @Robustness: another use-case for growing array?

	//
	// Check how many textures we have to load.
	//
	s32 textures_to_load_count = 0;
	auto& prepared_textures = Context.prepared_resources->textures;

	for ( s32 i = 0; i < textures.size(); ++i ) {
		constant result = linear_find_if( prepared_textures, [&]( Texture const& texture ) {
			return texture.name_hash == textures[i];
		} );

		if ( !result.found() ) {
			++textures_to_load_count;
		}
	}

	if ( textures_to_load_count > 0 ) {
		con_log_indented( 1, "Found % textures to load." );
	} else {
		con_log_indented( 1, "No textures to load this time." );
	}

	//
	// Check how many textures we have to free (excluding default ones), and
	// free them.
	//
	s32 textures_to_free_count = 0;
	for ( s32 i = defaults.textures.size()-1; i < prepared_textures.size(); ++i ) {
		constant result = linear_find( textures, prepared_textures[i].name_hash );

		if ( !result.found() ) {
			++textures_to_free_count;
			glDeleteTextures( 1, &prepared_textures[i].id );
		}
	}

	if ( textures_to_free_count > 0 ) {
		con_log_indented( 1, "Found % textures to free." );
	} else {
		con_log_indented( 1, "No textures to free this time." );
	}

	// Continue here. I stopped to add a growing array ffs.

	return true;
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