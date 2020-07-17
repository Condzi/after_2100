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
// Basic info about the planet which is needed for simulating
// orbiting.
// Don't confuse with Planet_Resource_Data, defined in engine/resource_types.hpp!!!
struct Planet_Info final
{
	v2 position; // center
	f32 radius; // in pixels; we can't go into lower orbit than radius * 1.1 (or other constant, maybe move it to macro_config?)
};

struct Planet final
{
	ENTITY_DEF( Planet );

	f32 radius = -1;
	// @ToDo: Maybe move it to _cold since more entities may want 
	// to have hashes of their names?
	u32 name_hash = 0;

	void initialize( Planet_Resource_Data const& planet_resource_data )
	{
		auto& resources  = *Context.prepared_resources;
		constant texture = resources.get_texture( planet_resource_data.texture_name_hash );
		constant shader  = resources.get_shader( "sprite_default"_hcs );

		radius = planet_resource_data.radius;
		name_hash = planet_resource_data.planet_name_hash;

		auto& render_info = _cold.basic_render_info;

		// @Robustness: right now we use 2*radius, but maybe in
		// the future we may need some other sizes? That would
		// be needed especially when we would be working with texture
		// atlases.
		render_info = construct_textured_sprite( 2*radius, 2*radius );
		render_info.texture = texture;
		render_info.shader  = shader;

		_hot.update_model_matrix = true;
	}


	void shutdown()
	{
		shutdown_textured_sprite( _cold.basic_render_info );
	}

	returning get_planet_info() const -> Planet_Info
	{
		return { .position = _hot.position, .radius = radius };
	}
};
}