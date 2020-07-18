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
struct Debug_Entity final
{
	ENTITY_DEF( Debug_Entity );

	void initialize()
	{
		// nothing?
	}

	void shutdown()
	{
		shutdown_ellipse( _cold.basic_render_info );
	}

	bool first_init = true;
	void update_ellipse( f32 horizontal, f32 vertical, v2 center )
	{
		auto& render_info = _cold.basic_render_info;

		if ( !first_init ) {
			shutdown_ellipse( render_info );
		}
		auto& resources  = *Context.prepared_resources;


		first_init = false;

		_hot.position = center;
		render_info = construct_ellipse( horizontal, vertical );
		render_info.shader = resources.get_shader( "ellipse"_hcs );
		render_info.drawing_group = Drawing_Group::Ellipse;

		_hot.update_model_matrix = true;
	}
};
}