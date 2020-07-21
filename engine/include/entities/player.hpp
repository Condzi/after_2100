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

	compile_constant planet_mass = 2.4e16f;
	compile_constant player_mass = 10.0f;
	compile_constant planet_radius = 64.0f;
	compile_constant G = 6.67e-11f;

	compile_constant acceleration_force = 80.0f;

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

		orbit_path = em.spawn_entity<Debug_Entity>();
		update_orbit_path();
	}

	void shutdown()
	{
		shutdown_textured_sprite( _cold.basic_render_info );
	}

	Debug_Entity* orbit_path;
	Planet_Info origin_planet;


	void physic_update( f32 ups )
	{
		auto& position = _hot.position;

		// player->planet
		constant vector = origin_planet.position - position;

		constant r = glm::length( vector );
		constant force = ( G * planet_mass * player_mass ) / ( r*r ) * ups;
		constant direction = glm::normalize( vector );

		constant force_v2 = direction * force;
		constant velocity = force_v2 / player_mass;
		_hot.velocity += velocity;
		position += _hot.velocity * ups;
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
			update_orbit_path();
		}

		_hot.update_model_matrix = true;
	}

	void update_orbit_path()
	{
		//
		// We just simulate the entire flight here. 
		// We probably should have an limiter later to not simulate flying to the outer
		// space for example.
		// Idea: use the data gathered here to just move the ship IRL, since we simulated 
		// entire flight here.
		//
		// Also, this shouldn't be here because when we will have additional planets we can't
		// use the `origin_planet.position` anymore. Therefore, this should be an entire separete
		// structure. Maybe called `Orbit_Simulator` or something.
		//

		constant distance_squared = []( v2 const& a, v2 const& b ) {
			constant x = a.x - b.x;
			constant y = a.y - b.y;

			return x*x + y*y;
		};

		// @ToDo: move to the local.variables? 
		// Or maybe should match the ups value?
		constant step = 1/60.0f;
		constant planet_position = origin_planet.position;
		constant starting_point = _hot.position;

		v2 current_position = _hot.position;
		v2 current_velocity = _hot.velocity;

		//
		// First, find out how many points do we need. Then loop again to
		// populate the array.
		// @Performance: we can just set the points count to be a very high number
		// and later just shrink the array. Right now we don't really care that much.
		//

		f32 steps_accumulated = 0;
		// Do at least 20 steps. Without that we may end up stopping too early for really tiny orbits.
		// However, this doesn't seem like an ideal solution either...
		while ( distance_squared( current_position, starting_point ) > 25 || steps_accumulated / step < 20 ) {
			constant vector = origin_planet.position - current_position;

			constant r = glm::length( vector );
			constant force = ( G * planet_mass * player_mass ) / ( r*r ) * step;
			constant direction = glm::normalize( vector );

			constant force_v2 = direction * force;
			constant velocity = force_v2 / player_mass;
			current_velocity += velocity;
			current_position += current_velocity * step;

			steps_accumulated += step;
		}


		constant points_count = ceil( steps_accumulated / step );
		Array<v2> points;
		points.initialize( points_count, Context.temporary_allocator );

		current_position = _hot.position;
		current_velocity = _hot.velocity;

		for ( s32 i = 0; i < points_count; ++i ) {
			constant vector = origin_planet.position - current_position;

			constant r = glm::length( vector );
			constant force = ( G * planet_mass * player_mass ) / ( r*r ) * step;
			constant direction = glm::normalize( vector );

			constant force_v2 = direction * force;
			constant velocity = force_v2 / player_mass;
			current_velocity += velocity;
			current_position += current_velocity * step;

			points[i] = current_position;
		}

		//
		// Initialize Render_Info.
		//
		auto& ri = orbit_path->_cold.basic_render_info;
		ri.render_type = Render_Type::Draw_Arrays;
		ri.draw_arrays_info.mode = GL_LINE_LOOP;
		ri.draw_arrays_info.vertices_count = points_count;

		if ( ri.vao != 0 ) {
			glDeleteVertexArrays( 1, &ri.vao );
			glDeleteBuffers( 1, &ri.vbo );
		}

		glGenVertexArrays( 1, &ri.vao );
		glGenBuffers( 1, &ri.vbo );

		glBindVertexArray( ri.vao );

		glBindBuffer( GL_ARRAY_BUFFER, ri.vbo );
		glBufferData( GL_ARRAY_BUFFER, sizeof( Position_Vertex2D )* points_count, points.data(), GL_STATIC_DRAW );

		// position 
		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, 0 );
		glEnableVertexAttribArray( 0 );

		ri.shader = Context.prepared_resources->get_shader( "primitive"_hcs );
	}
};
}