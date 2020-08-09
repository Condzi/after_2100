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
		con_push_indent();
		con_log( "Error: texture of hash % not found!", name.hash );
		con_pop_indent();

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
		con_push_indent();
		con_log( "Error: shader  of hash % not found!", name.hash );
		con_pop_indent();
		return fallback_shader;
	}
}

returning Prepared_Resources::get_font( Hashed_CString name ) -> Font&
{
	con_assert( fonts.size() > 0 );

	constant result = linear_find( fonts_names_hashes, name.hash );

	if ( result.found() ) {
		return fonts[result.idx];
	} else {
		con_push_indent();
		con_log( "Error: font    of hash % not found!", name.hash );
		con_pop_indent();
		return fallback_font;
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