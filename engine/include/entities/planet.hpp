#pragma once

#include <engine/entity.hpp>
#include <engine/prepared_resources.hpp>
#include <engine/resource_types.hpp>
#include <engine/context.hpp>
#include <engine/logger.hpp>

#include <engine/renderer.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace con
{
struct Planet_Earth final
{
	ENTITY_DEF( Planet_Earth );

	void initialize()
	{
		auto& resources  = *Context.prepared_resources;
		constant texture = resources.get_texture( "planet_earth"_hcs );
		constant shader  = resources.get_shader( "sprite_default"_hcs );

		auto& render_info = _cold.basic_render_info;

		render_info = construct_textured_sprite( 0, 0 );
		render_info.texture = texture;
		render_info.shader  = shader;
	}


	void shutdown()
	{
		shutdown_textured_sprite( _cold.basic_render_info );
	}
};
}