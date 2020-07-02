#include <engine/prepared_resources.hpp>
#include <engine/algorithms.hpp>

namespace con
{
returning Prepared_Resources::get_texture( Hashed_CString name ) -> Texture
{
	/*
	constant result = binary_find_if( textures, [&name]( Texture const& a ) {
		return a.name_hash < name.hash;
	}, [&name]( Texture const& a ) {
		return a.name_hash == name.hash;
	} );

	if ( result.found() ) {
		return textures[result.idx];
	} else {
		release_con_assert( false );
		return {};
	}
	*/

	con_assert( textures.size() > 0 );

	constant result = linear_find_if( textures, [&name]( constant& a ) {
		return a.name_hash == name.hash;
	} );

	if ( result.found() ) {
		return textures[result.idx];
	} else {
		release_con_assert( false );
		return {};
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
		release_con_assert( false );
		return {};
	}
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