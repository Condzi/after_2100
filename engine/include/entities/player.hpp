#pragma once

#include <engine/entity.hpp>
#include <engine/prepared_resources.hpp>
#include <engine/resource_types.hpp>
#include <engine/context.hpp>
#include <engine/logger.hpp>

#include <engine/renderer.hpp>
#include <engine/input.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace con
{
struct Player final
{
	ENTITY_DEF( Player );

	void initialize()
	{
		auto& resources  = *Context.prepared_resources;
		constant texture = resources.get_texture( "player"_hcs );
		constant shader  = resources.get_shader( "sprite_default"_hcs );

		auto& render_info = _cold.basic_render_info;

		render_info = construct_textured_sprite( 140, 108 );
		render_info.texture = texture;
		render_info.shader = shader;

		//	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		Context.input->add_binding( "enlarge"_hcs, con::Key::Keyboard_E );
		Context.input->add_binding( "decrease"_hcs, con::Key::Keyboard_D );
	}


	void shutdown()
	{
		shutdown_textured_sprite( _cold.basic_render_info );
	}


	f32 accumulated_ups = 0;
	f32 radius = 100.0f;
	compile_constant radius_delta = 100.0f;
	compile_constant origin_x = 1280/2.0f;
	compile_constant origin_y = 720/2.0f;


	void physic_update( f32 ups )
	{
		accumulated_ups += ups;

		auto& position = _hot.position;

		position.x = origin_x + cosf( accumulated_ups /* * velocity on orbit */ ) * radius;
		position.y = origin_y + sinf( accumulated_ups ) * radius;
	}

	void frame_update( f32 dt )
	{
		constant pos = _hot.position;
		constant PI_correction = 3.1415 / 2;
		constant angle = atan2f( pos.y - origin_y, pos.x - origin_x );
		_hot.rotation_z = angle + PI_correction;


		auto& model_mat = _cold.basic_render_info.model_mat;
		model_mat = mat4{ 1.0f };
		model_mat = glm::translate( model_mat, v3{ _hot.position.x, _hot.position.y,  0 } );
		model_mat = glm::rotate( model_mat, _hot.rotation_z, v3{ 0.0f, 0.0f, 1.0f } );

		if ( Context.input->is_key_held( "enlarge"_hcs ) ) {
			if ( radius <= 400 ) {
				radius += radius_delta * dt;
			} else {
				radius = 400;
			}
		}

		if ( Context.input->is_key_held( "decrease"_hcs ) ) {
			if ( radius >= 100 ) {
				radius -= radius_delta * dt;
			} else {
				radius = 100;
			}
		}

	}
};
}