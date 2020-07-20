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
#include <entities/debug_entity.hpp>

// min/max
#include <algorithm>

namespace con
{
struct Player final
{
	ENTITY_DEF( Player );

	compile_constant planet_mass = 1.474e13f;
	compile_constant player_mass = 10.0f;
	compile_constant planet_radius = 64.0f;
	compile_constant G = 6.67e-11f;

	compile_constant acceleration_force = 30.0f;

	void initialize()
	{
		auto& resources  = *Context.prepared_resources;
		constant texture = resources.get_texture( "player"_hcs );
		constant shader  = resources.get_shader( "sprite_default"_hcs );

		auto& render_info = _cold.basic_render_info;

		render_info = construct_textured_sprite( 48, 48 );
		render_info.texture = texture;
		render_info.shader  = shader;

		//	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		Context.input->add_binding( "acceleration"_hcs, Key::Keyboard_SPACE );

		constant starting_planet_hash = resources.get_starting_planet_hash();
		auto& em = *Context.entity_manager;

		constant starting_planet_entity_find_result = linear_find_if( em.by_type.planet, [&starting_planet_hash]( Planet const* planet_ptr ) {
			return planet_ptr != nullptr && planet_ptr->name_hash == starting_planet_hash;
		} );

		// @ToDo: Better error handling here!!!
		con_assert( starting_planet_entity_find_result.found() );

		origin_planet = em.by_type.planet[starting_planet_entity_find_result.idx]->get_planet_info();

		// Set player above the planet.
		_hot.position = origin_planet.position;
		_hot.position -= v2{ 0, 150 };

		constant r = glm::distance( _hot.position, origin_planet.position );
		_hot.velocity.x = sqrtf( G * planet_mass / r );
	}

	void shutdown()
	{
		shutdown_textured_sprite( _cold.basic_render_info );
	}


	Planet_Info origin_planet;


	void physic_update( f32 ups )
	{
		auto& position = _hot.position;

		// player->planet
		constant vector = origin_planet.position - position;

		constant r = glm::length( vector );
		constant force = ( G * planet_mass * player_mass ) / ( r*r );
		constant direction = glm::normalize( vector );

		constant force_v2 = direction * force;
		constant velocity = force_v2 / player_mass;
		_hot.velocity += velocity;
		position += _hot.velocity;
	}

	void frame_update( f32 dt )
	{
		constant pos = _hot.position;
		constant PI_correction = 3.1415f * 3/2;

		constant mouse_position = Context.input->get_mouse_position();

		constant angle = atan2f( pos.y - mouse_position.y, pos.x - mouse_position.x );
		_hot.rotation_z = angle + PI_correction;

		// This shouldn't be calculated / added here, but in the fixed_update instead.
		if ( Context.input->is_key_held( "acceleration"_hcs ) ) {
			constant vector = mouse_position - pos;
			constant direction = glm::normalize( vector );
			constant force_v2 = direction * acceleration_force;
			constant velocity = force_v2 / player_mass;

			_hot.velocity += velocity * dt;
		}

		_hot.update_model_matrix = true;
	}
};
}