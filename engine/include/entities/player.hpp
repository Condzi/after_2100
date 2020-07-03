#pragma once

#include <engine/typedefs.hpp>
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
	Entity::Hot& _hot;
	Entity::Cold& _cold;

	// @Cleanup: can be a macro, actually.
	Player( Entity::Hot& hot, Entity::Cold& cold ) :
		_hot( hot ),
		_cold( cold )
	{
		_hot.data = this;
		// Entity_Type should have the same name as class?
		// This way we could have simple ENTITY_DEF( Player ) macro.
		_hot.type = Entity_Type::Player; 
	}

	void initialize()
	{
		auto& resources  = *Context.prepared_resources;
		constant texture = resources.get_texture( "player"_hcs );
		constant shader  = resources.get_shader( "sprite_default"_hcs );

		auto& render_info = _cold.basic_render_info;

		render_info = Render_Info{};

		render_info.texture = texture;
		render_info.shader = shader;
		render_info.elements_count = 6; // 6 elemets in ebo for a quad

		// @Robustness: abstract this stuff away, we should be able to call create_sprite()
		// or something like that. 

		// Use actual texture dimensions here, gathered from Resource_Loader?
		auto quad = construct_2d_textured_quad( 140, 108 );

		glGenVertexArrays( 1, &render_info.vao );
		glGenBuffers( 1, &render_info.vbo );

		glBindVertexArray( render_info.vao );

		glBindBuffer( GL_ARRAY_BUFFER, render_info.vbo );
		glBufferData( GL_ARRAY_BUFFER, sizeof( Textured_Vertex2D ) * 4, quad.data(), GL_STATIC_DRAW );

		// position 
		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( Textured_Vertex2D ), (void*)0 );
		glEnableVertexAttribArray( 0 );
		// texture coord
		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( Textured_Vertex2D ), (void*)( 2* sizeof( f32 ) ) );
		glEnableVertexAttribArray( 1 );

		// Initialize element buffer and use only one for every sprite.
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, Context.renderer->get_quad_ebo() );

		glBindVertexArray( 0 );

		//	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		Context.input->add_binding( "enlarge"_hcs, con::Key::Keyboard_E );
		Context.input->add_binding( "decrease"_hcs, con::Key::Keyboard_D );
	}


	void shutdown()
	{
		glDeleteVertexArrays( 1, &_cold.basic_render_info.vao );
		glDeleteBuffers( 1, &_cold.basic_render_info.vbo );
	}


	f32 accumulated_ups = 0;
	f32 radius = 100.0f;
	compile_constant radius_delta = 100.0f;

	void physic_update( f32 ups )
	{
		constant origin_x = 1280/2.0f;
		constant origin_y = 720/2.0f;

		accumulated_ups += ups;

		auto& position = _hot.position;

		position.x = origin_x + cosf( accumulated_ups /* * velocity on orbit */ ) * radius;
		position.y = origin_y + sinf( accumulated_ups ) * radius;
	}

	void frame_update( f32 dt )
	{
		auto& model_mat = _cold.basic_render_info.model_mat;
		model_mat = mat4{ 1.0f };
		model_mat = glm::translate( model_mat, v3{ _hot.position.x, _hot.position.y,  0 } );

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