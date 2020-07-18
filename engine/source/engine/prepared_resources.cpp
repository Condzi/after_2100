#include <engine/prepared_resources.hpp>
#include <engine/algorithms.hpp>
#include <engine/logger.hpp>

namespace con
{
returning Prepared_Resources::get_texture( Hashed_CString name ) -> Texture
{
	con_assert( textures.size() > 0 );

	constant result = linear_find_if( textures, [&name]( constant& a ) {
		return a.name_hash == name.hash;
	} );

	if ( result.found() ) {
		return textures[result.idx];
	} else {
		con_log_indented( 1, "Error: texture of hash % not found!", name.hash );

		return fallback_texture;
	}
}
returning Prepared_Resources::get_shader( Hashed_CString name ) -> Shader
{
	con_assert( shaders.size() > 0 );

	constant result = linear_find_if( shaders, [&name]( constant& a ) {
		return a.name_hash == name.hash;
	} );

	if ( result.found() ) {
		return shaders[result.idx];
	} else {
		con_log_indented( 1, "Error: shader of hash % not found!", name.hash );
		return fallback_shader;
	}
}

returning Prepared_Resources::get_starting_planet_hash() const -> u32
{
	return starting_planet_hash;
}

returning Prepared_Resources::textures_count() const -> s32
{
	return textures.size();
}

returning Prepared_Resources::shaders_count() const -> s32
{
	return shaders.size();
}
}