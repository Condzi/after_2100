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

		debug_entity = Context.entity_manager->spawn_entity<Debug_Entity>();
		update_orbit();
	}

	// We use the debug entity for drawig the ellipse.
	Debug_Entity* debug_entity = nullptr;

	void shutdown()
	{
		shutdown_textured_sprite( _cold.basic_render_info );
	}

	// It's a paremeter for ellipse. We change it depending
	// on the speed.
	f32 theta = 0;
	Planet_Info origin_planet;

	f32 orbit_x = 200;
	f32 orbit_y = 164;
	compile_constant orbit_change_delta = 50.0f;

	v2 shift ={ 0,0 };

	void update_orbit()
	{
		constant& origin_x = origin_planet.position.x;
		constant& origin_y = origin_planet.position.y;

		constant A = orbit_x;
		constant B = orbit_y;

		constant F = sqrtf( fabs( A*A - B*B ) );
		// We have to keep track of the correct major axis of the ellipse!
		shift = v2( A>B ? ( origin_x + F ) : origin_x,
					B>A ? ( origin_y + F ) : origin_y );

		// @ToDo: update with the SHIFT value only when orbit changes!
		debug_entity->update_ellipse( orbit_x, orbit_y, shift );
	}

	void physic_update( f32 ups )
	{
		auto& position = _hot.position;


		constant distance_between_planet_and_player = glm::distance( position, origin_planet.position );

		// G * Mass of the planet
		compile_constant magic_multiplier = 100;
		// We need this sqrtf. At least for now, without it the movement is meh, even with
		// OK constants.
		constant velocity = sqrtf( magic_multiplier / distance_between_planet_and_player );

		theta += velocity * ups;
		if ( theta > 2 * 3.1415f ) {
			theta -= 2 * 3.1415f;
		}


		constant A = orbit_x;
		constant B = orbit_y;

		position.x = A * cosf( theta ) + shift.x;
		position.y = B * sinf( theta ) + shift.y;
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
		v2 orbit_delta{ 0,0 };

		if ( Context.input->is_key_held( "X+"_hcs ) ) {
			orbit_delta.x += change;
		}

		if ( Context.input->is_key_held( "X-"_hcs ) ) {
			orbit_delta.x -= change;
		}

		if ( Context.input->is_key_held( "Y+"_hcs ) ) {
			orbit_delta.y += change;
		}

		if ( Context.input->is_key_held( "Y-"_hcs ) ) {
			orbit_delta.y -= change;
		}

		if ( orbit_delta.x != 0 || orbit_delta.y != 0 ) {
			orbit_x += orbit_delta.x;
			orbit_y += orbit_delta.y;

			update_orbit();
		}

		_hot.update_model_matrix = true;
	}
};
}