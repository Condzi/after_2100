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

// min/max
#include <algorithm>

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

		render_info = construct_textured_sprite( 48, 48 );
		render_info.texture = texture;
		render_info.shader = shader;

		//	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		Context.input->add_binding( "X+"_hcs, con::Key::Keyboard_D );
		Context.input->add_binding( "X-"_hcs, con::Key::Keyboard_A );

		Context.input->add_binding( "Y+"_hcs, con::Key::Keyboard_S );
		Context.input->add_binding( "Y-"_hcs, con::Key::Keyboard_W );

		constant starting_planet_hash = resources.get_starting_planet_hash();
		auto& em = *Context.entity_manager;

		constant starting_planet_entity_find_result = linear_find_if( em.by_type.planet, [&starting_planet_hash]( Planet const* planet_ptr ) {
			return planet_ptr != nullptr && planet_ptr->name_hash == starting_planet_hash;
		} );

		// @ToDo: Better error handling here!!!
		con_assert( starting_planet_entity_find_result.found() );

		origin_planet = em.by_type.planet[starting_planet_entity_find_result.idx]->get_planet_info();
	}


	void shutdown()
	{
		shutdown_textured_sprite( _cold.basic_render_info );
	}


	f32 accumulated_ups = 0;
	Planet_Info origin_planet;

	f32 orbit_x = 200;
	f32 orbit_y = 164;
	compile_constant orbit_change_delta = 50.0f;

	void physic_update( f32 ups )
	{
		accumulated_ups += ups;

		auto& position = _hot.position;
		constant& origin_x = origin_planet.position.x;
		constant& origin_y = origin_planet.position.y;

		constant A = std::max( orbit_x, orbit_y );
		constant B = std::min( orbit_x, orbit_y );

		constant F = sqrtf( A*A - B*B );
		constant SHIFT = v2( origin_x + F, origin_y );

		position.x = A * cosf( accumulated_ups ) + SHIFT.x;
		position.y = B * sinf( accumulated_ups ) + SHIFT.y;
	}

	void frame_update( f32 dt )
	{
		constant pos = _hot.position;
		constant PI_correction = 3.1415f * 3/2;
		constant& planet_radius = origin_planet.radius;

		constant mouse_position = Context.input->get_mouse_position();

		constant angle = atan2f( pos.y - mouse_position.y, pos.x - mouse_position.x );
		_hot.rotation_z = angle + PI_correction;

		constant change = orbit_change_delta * dt;

		if ( Context.input->is_key_held( "X+"_hcs ) ) {
			orbit_x += change;
		}

		if ( Context.input->is_key_held( "X-"_hcs ) ) {
			orbit_x -= change;
		}

		if ( Context.input->is_key_held( "Y+"_hcs ) ) {
			orbit_y += change;
		}

		if ( Context.input->is_key_held( "Y-"_hcs ) ) {
			orbit_y -= change;
		}

		con_log_indented( 2, "orbit_x = %", orbit_x );
		con_log_indented( 2, "orbit_y = %", orbit_y );
		con_log_indented( 2, "=====================" );

		_hot.update_model_matrix = true;
	}
};
}