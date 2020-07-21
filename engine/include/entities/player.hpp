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
		// 1. Find point that is closest to the planet
		// 2. Start gathering points.
		// 3. Gather until you're past the point which is closest to the planet.
		//

		constant distance_squared = []( v2 const& a, v2 const& b ) {
			constant x = a.x - b.x;
			constant y = a.y - b.y;

			return x*x + y*y;
		};

		constant step = 1/20.0f;
		constant planet_position = origin_planet.position;
		constant starting_point = _hot.position;

		v2 current_position = _hot.position;
		v2 current_velocity = _hot.velocity;
		f32 highest_distance = distance_squared( current_position, planet_position );
		f32 lowest_distance  = distance_squared( current_position, planet_position );

		v2 furtherest_point = current_position;
		v2 closest_point = current_position;

		// Search until we're still finding extreme points.
		bool still_searching = true;
		f32 steps_accumulated = 0;
		do {
			constant vector = origin_planet.position - current_position;

			constant r = glm::length( vector );
			constant force = ( G * planet_mass * player_mass ) / ( r*r ) * step;
			constant direction = glm::normalize( vector );

			constant force_v2 = direction * force;
			constant velocity = force_v2 / player_mass;
			current_velocity += velocity;
			current_position += current_velocity * step;

			constant distance = distance_squared( current_position, planet_position );
			if ( distance < lowest_distance ) {
				lowest_distance = distance;
				closest_point = current_position;
			}

			if ( distance > highest_distance ) {
				highest_distance = distance;
				furtherest_point = current_position;
			}
			steps_accumulated += step;
			// Do at least 20 steps
		} while ( distance_squared( current_position, starting_point ) > 25 || steps_accumulated / step < 20 );


		//constant major_axis_length = glm::distance( furtherest_point, closest_point );
		// 1 point every 10 pixels
		//constant points_count = 2 * ( major_axis_length / 10 );
		constant points_count = ceil( steps_accumulated / step );
		Array<v2> points;
		points.initialize( points_count, Context.temporary_allocator );

		con_log( "Points: %", points_count );

		current_position = _hot.position;
		current_velocity = _hot.velocity;

		for ( s32 i = 0; i < points_count - 1; ++i ) {
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

		points[points_count - 1] = points[0];

		//
		// Initialize Render_Info.
		//
		auto& ri = orbit_path->_cold.basic_render_info;
		ri.render_type = Render_Type::Draw_Arrays;
		ri.draw_arrays_info.mode = GL_LINE_STRIP;
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

		ri.shader = Context.prepared_resources->get_shader( "ellipse"_hcs );
	}
};
}