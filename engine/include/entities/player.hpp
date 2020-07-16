#pragma once

#include <engine/entity.hpp>
#include <engine/prepared_resources.hpp>
#include <engine/resource_types.hpp>
#include <engine/context.hpp>
#include <engine/logger.hpp>

#include <engine/renderer.hpp>
#include <engine/input.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <entities/planet.hpp>

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

		constant starting_planet_hash = resources.get_starting_planet_hash();
		auto& em = *Context.entity_manager;

		constant starting_planet_entity_find_result = linear_find_if( em.by_type.planet, [&starting_planet_hash]( Planet const* planet_ptr ) {
			return planet_ptr != nullptr && planet_ptr->name_hash == starting_planet_hash;
		} );

		// @ToDo: Better error handling here!!!
		con_assert( starting_planet_entity_find_result.found() );

		origin_planet = em.by_type.planet[starting_planet_entity_find_result.idx]->get_planet_info();

		current_radius = origin_planet.radius * 1.2;
	}


	void shutdown()
	{
		shutdown_textured_sprite( _cold.basic_render_info );
	}


	f32 accumulated_ups = 0;
	compile_constant radius_delta = 100.0f;
	f32 current_radius = 100.0f;
	Planet_Info origin_planet;


	void physic_update( f32 ups )
	{
		accumulated_ups += ups;

		auto& position = _hot.position;
		constant& origin_x = origin_planet.position.x;
		constant& origin_y = origin_planet.position.y;

		position.x = origin_x + cosf( accumulated_ups /* * velocity on orbit */ ) * current_radius;
		position.y = origin_y + sinf( accumulated_ups ) * current_radius;
	}

	void frame_update( f32 dt )
	{
		constant pos = _hot.position;
		constant PI_correction = 3.1415f / 2;
		constant& origin_x = origin_planet.position.x;
		constant& origin_y = origin_planet.position.y;
		constant& planet_radius = origin_planet.radius;
		constant angle = atan2f( pos.y - origin_y, pos.x - origin_x );
		_hot.rotation_z = angle + PI_correction;

		// @Performance / @Cleanup:
		// Move matrix calculation to separate loop in entity_manager.update?
		auto& model_mat = _cold.basic_render_info.model_mat;
		model_mat = mat4{ 1.0f };
		model_mat = glm::translate( model_mat, v3{ pos.x, pos.y,  0 } );
		model_mat = glm::rotate( model_mat, _hot.rotation_z, v3{ 0.0f, 0.0f, 1.0f } );

		if ( Context.input->is_key_held( "enlarge"_hcs ) ) {
			if ( current_radius <= planet_radius * 5 ) {
				current_radius += radius_delta * dt;
			} else {
				current_radius = 400;
			}
		}

		if ( Context.input->is_key_held( "decrease"_hcs ) ) {
			if ( current_radius >= planet_radius * 1.2 ) {
				current_radius -= radius_delta * dt;
			} else {
				current_radius = planet_radius * 1.2;
			}
		}

	}
};
}