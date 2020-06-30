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

struct Fallback_Color final
{
	compile_constant r = static_cast<u8>( CON_FALLBACK_TEXTURE_COLOR_R );
	compile_constant g = static_cast<u8>( CON_FALLBACK_TEXTURE_COLOR_G );
	compile_constant b = static_cast<u8>( CON_FALLBACK_TEXTURE_COLOR_B );
	compile_constant a = static_cast<u8>( CON_FALLBACK_TEXTURE_COLOR_A );
};

// Allocates using temporary allocator.
returning generate_sized_fallback_texture( s32 size ) -> byte*
{
	auto& ta = *reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );

	// 1 byte per color, RGBA colors.
	auto* data = ta.allocate<Fallback_Color>( size );

	for ( s32 i = 0; i < size; ++i ) {
		data[i] = Fallback_Color{};
	}

	return reinterpret_cast<byte*>( data );
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

	// Create the fallback texture
	// @Incomplete: create fallback shader?

	fallback.texture.name_hash = 0xdeadbeef;
	fallback.texture.id = init_texture( generate_sized_fallback_texture( CON_FALLBACK_TEXTURE_SIZE * CON_FALLBACK_TEXTURE_SIZE ), CON_FALLBACK_TEXTURE_SIZE, CON_FALLBACK_TEXTURE_SIZE );

	check_scene_folder_content();

	//
	// Getting assets metadata 
	//
	con_log_indented( 1, "Loading resource metadata from \"%\"...", CString{ CON_ASSETS_CONFIG_FILE } );
	Config_File assets_config;
	defer{ assets_config.free(); };
	assets_config.parse_from_file( CON_ASSETS_CONFIG_FILE );
	// Config_File::get_section returns temporary storage memory, so we can reset it after we
	// use the array.
	auto& da = *reinterpret_cast<Default_Allocator*>( Context.default_allocator );
	auto& ta = *reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );

	// Handling textures metadata.
	{
		auto ta_mark = ta.get_mark();
		defer{ ta.set_mark( ta_mark ); };
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
	}

	// Handling shaders metadata.
	{
		auto ta_mark = ta.get_mark();
		defer{ ta.set_mark( ta_mark ); };
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
					con_log_indented( 1, R"(Error: can't format the shader path "%"!)", it.value );
				} else {
					char* path_memory = da.allocate<char>( path.size );
					memcpy( path_memory, path.data, path.size );
					paths.shaders[current_shader_idx] ={ path_memory, path.size };
				}
				++current_shader_idx;
			}
		}
	}

	con_log_indented( 1, "Metadata loaded." );

	//
	// Loading default resources info specified in default.scene_resources
	//

	con_log_indented( 1, R"(Loading default resources info from "%".)", CString{ CON_DEFAULT_SCENE_RESOURCES_INFO_FILE } );
	defaults.textures.shutdown();
	defaults.fonts.shutdown();
	defaults.shaders.shutdown();

	Array<u32> textures_hash, fonts_hash, shaders_hash;

	if ( !parse_scene_resources_file( CON_DEFAULT_SCENE_RESOURCES_INFO_FILE, textures_hash, fonts_hash, shaders_hash ) ) {
		con_log_indented( 1, "Error: parsing failed!" );
		return;
	} else {
		con_log_indented( 1, "Parsing succeed." );
	}

	//
	// Loading resources data and creating OpenGL stuff for them.
	//

	if ( textures_hash.size() > 0 ) {
		con_log_indented( 1, "Loading default textures..." );

		auto& default_textures = defaults.textures;
		default_textures.initialize( textures_hash.size() );

		constant ta_mark = ta.get_mark();
		for ( s32 i = 0; i < defaults.textures.size(); ++i ) {
			defer{ ta.set_mark( ta_mark ); };
			auto& current_texture = default_textures[i];
			current_texture.name_hash = textures_hash[i];

			constant result = linear_find( name_hashes.textures, current_texture.name_hash );

			if ( !result.found() ) {
				con_log_indented( 2, "Error: can't find texture (hash %, i = %).", current_texture.name_hash, i );
				continue;
			}

			constant idx = result.idx;

			constant[texture_width, texture_height] = texture_data[idx];

			auto data = load_texture_data( paths.textures[idx], texture_width, texture_height );

			if ( data == nullptr ) {
				data = generate_sized_fallback_texture( texture_width * texture_height );
			}

			current_texture.id = init_texture( data, texture_width, texture_height );
		}
	} else {
		con_log_indented( 1, "No default textures to load." );
	}

	con_log_indented( 1, "Loading default fonts... (@INCOMPLETE, dummy)" );

	if ( shaders_hash.size() > 0 ) {
		con_log_indented( 1, "Loading default shaders..." );

		auto& default_shaders = defaults.shaders;
		default_shaders.initialize( shaders_hash.size() );

		constant ta_mark = ta.get_mark();

		for ( s32 i = 0; i < defaults.shaders.size(); ++i ) {
			defer{ ta.set_mark( ta_mark ); };
			auto& current_shader = defaults.shaders[i];

			current_shader.name_hash = shaders_hash[i];

			constant result = linear_find( name_hashes.shaders, current_shader.name_hash );

			if ( !result.found() ) {
				con_log_indented( 2, "Error: can't find shader (hash %, i = %).", current_shader.name_hash, i );
				continue;
			}

			constant idx = result.idx;

			constant source = load_shader_source( paths.shaders[idx] );

			if ( source.size > 0 ) {
				current_shader.id = build_shader_program( source );
			} else {
				con_log_indented( 2, "Error: no shader source (hash %, idx = %)", current_shader.name_hash, idx );
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

	for ( s32 i = 0; i < defaults.textures.size(); ++i ) {
		glDeleteTextures( 1, &defaults.textures[i].id );
	}

	for ( s32 i = 0; i < defaults.shaders.size(); ++i ) {
		glDeleteProgram( defaults.shaders[i].id );
	}

	glDeleteTextures( 1, &fallback.texture.id );
	//glDeleteProgram( fallback.shader.id );

	defaults.textures.shutdown();
	defaults.fonts.shutdown();
	defaults.shaders.shutdown();


	scene_folder_content.hashes.shutdown();

	texture_data.shutdown();
}

void Resource_Loader::reload()
{
	// @Incomplete: implement reload here!!!!
}

returning Resource_Loader::prepare_resources_for_scene( CString scene_name ) -> bool
{
	//
	// Check if this scene file exists (we could use the filesystem here too?)
	//
	{
		constant scene_name_hash = hash_cstring( scene_name );

		constant result = linear_find( scene_folder_content.hashes, scene_name_hash );
		if ( !result.found() ) {
			con_log_indented( 1, R"(Error: can't find resource file for scene "%". Is name correct?)", scene_name );
			return false;
		}
	}

	//
	// Get list of resources that scene wants to have.
	//
	constant scene_file_path = sprint( "%%%", CString{ CON_SCENES_FOLDER }, scene_name, CString{ CON_SCENE_RESOURCES_FILE_EXTENSION } );

	Array<u32> r_textures, r_fonts, r_shaders; // r_ = requested

	if ( !parse_scene_resources_file( scene_file_path, r_textures, r_fonts, r_shaders ) ) {
		return false;
	}

	auto& p_textures = Context.prepared_resources->textures; // p_ = prepared
	constant default_textures_count = defaults.textures.size();

	// If it's first scene that we're loading, skip this step.
	if ( p_textures.size() > 0 ) {

		con_log_indented( 1, "Deleting % textures (leaving % defaults).", p_textures.size() - default_textures_count, default_textures_count );
		//
		// Delete all textures besides the default. (Default ones are at the beginning)
		//
		for ( s32 i = default_textures_count; i < p_textures.size(); ++i ) {
			constant texture_id = p_textures[i].id;

			glDeleteTextures( 1, &texture_id );
		}

		p_textures.shutdown();
	}

	p_textures.initialize( default_textures_count + r_textures.size() );

	//
	// Copy the default textures, then load the new data.
	// First, we have to find idx in name_hashes. Then load the texture and at last
	// create an OpenGL texture out of it.
	//

	memcpy( p_textures.data(), defaults.textures.data(), default_textures_count );

	// We're using TA to load textures and to generate fallback ones, so we can reset (free) 
	// the used memory by reseting the mark in every step of the loop.
	auto& ta = *reinterpret_cast<Temporary_Allocator*>( Context.temporary_allocator );

	for ( s32 i = default_textures_count, j = 0; i < p_textures.size(); ++i, ++j ) {
		constant mark = ta.get_mark();
		defer{ ta.set_mark( mark ); };

		auto current_texture = p_textures[i];
		current_texture.name_hash = r_textures[j];

		constant result = linear_find( name_hashes.textures, current_texture.name_hash );

		byte* data = nullptr;
		if ( !result.found() ) {
			data = generate_sized_fallback_texture( CON_FALLBACK_TEXTURE_SIZE * CON_FALLBACK_TEXTURE_SIZE );

			current_texture.id = init_texture( data, CON_FALLBACK_TEXTURE_SIZE, CON_FALLBACK_TEXTURE_SIZE );

			continue;
		}

		constant idx = result.idx;
		constant& path = paths.textures[idx];
		constant width = texture_data[idx].width;
		constant height = texture_data[idx].height;

		data = load_texture_data( path, width, height );

		if ( data == nullptr ) {
			data = generate_sized_fallback_texture( width * height );
		}

		current_texture.id = init_texture( data, width, height );
	}

	con_log_indented( 1, "Prepared % textures. (Total: %)", r_textures.size(), p_textures.size() );


	auto& p_shaders = Context.prepared_resources->shaders;
	constant default_shaders_count = defaults.shaders.size();

	// If it's first scene that we're loading, again, skip this step.
	if ( p_shaders.size() > 0 ) {
		con_log_indented( 1, "Deleting % shaders (leaving % defaults)", p_shaders.size() - default_shaders_count, default_shaders_count );

		// Delete shaders beside the default ones which are at the beginning.
		for ( s32 i = default_shaders_count; i < p_shaders.size(); ++i ) {
			glDeleteProgram( p_shaders[i].id );
		}

		p_shaders.shutdown();
	}

	//
	//	Copy the default shaders data and, if specified, load new ones.
	//

	p_shaders.initialize( default_shaders_count + r_shaders.size() );
	memcpy( p_shaders.data(), defaults.shaders.data(), default_shaders_count );

	if ( r_shaders.size() == 0 ) {
		con_log_indented( 1, "No shaders specified to load, only default are now present." );
	} else {
		constant ta_mark = ta.get_mark();

		for ( s32 i = default_shaders_count; i < p_shaders.size(); ++i ) {
			defer{ ta.set_mark( ta_mark ); };
			auto& current_shader = p_shaders[i];

			current_shader.name_hash = r_shaders[i];

			constant result = linear_find( name_hashes.shaders, current_shader.name_hash );

			if ( !result.found() ) {
				con_log_indented( 2, "Error: can't find shader (hash %, i = %).", current_shader.name_hash, i );
				continue;
			}

			constant idx = result.idx;

			constant source = load_shader_source( paths.shaders[idx] );

			if ( source.size > 0 ) {
				current_shader.id = build_shader_program( source );
			} else {
				con_log_indented( 2, "Error: no shader source (hash %, idx = %)", current_shader.name_hash, idx );
			}
		}

		con_log_indented( 1, "Prepared % shaders. (Total: %)", r_shaders.size(), p_shaders.size() );
	}

	return true;
}

void Resource_Loader::check_scene_folder_content()
{
	namespace fs = std::filesystem;

	scene_folder_content.hashes.shutdown();

	scene_folder_content.hashes.initialize( CON_MAX_SCENES_IN_FOLDER );
	//
	// Gathering the files count
	//
	std::error_code ec;
	auto directory_iterator = fs::directory_iterator( CON_SCENES_FOLDER, ec );
	if ( ec ) {
		con_log_indented( 2, R"(Error: can't iterate over scenes folder ("%"). Reason: "%".)", CString{ CON_SCENES_FOLDER }, cstring_from_stdstring( ec.message() ) );
		return;
	}

	s32 current_file = 0; // It's also the number of files in the folder.

	for ( auto& entry : directory_iterator ) {
		constant path = entry.path();
		if ( !path.has_extension() || !path.has_filename() ) {
			continue;
		}

		if ( strcmp( path.extension().generic_string().c_str(), CON_SCENE_RESOURCES_FILE_EXTENSION ) == 0 ) {

			constant filename = cstring_from_stdstring( path.filename().generic_string() );

			scene_folder_content.hashes[current_file] = hash_cstring( filename );

			++current_file;
		}
	}

	scene_folder_content.hashes.shrink( current_file );

	if ( current_file == 0 ) {
		con_log_indented( 2, "Warning: no scene files found." );
	} else {
		con_log_indented( 2, R"(Found % "%" files.)", current_file, CString{ CON_SCENE_RESOURCES_FILE_EXTENSION } );
	}
}

returning Resource_Loader::init_texture( byte* data, s16 width, s16 height ) -> gl_id
{
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

	constant texture_width = static_cast<s32>( width );
	constant texture_height = static_cast<s32>( height );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

	return texture_id;
}
}