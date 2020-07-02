#pragma once

#include <engine/typedefs.hpp>
#include <engine/entity.hpp>
#include <engine/prepared_resources.hpp>
#include <engine/resource_types.hpp>
#include <engine/context.hpp>

#include <engine/renderer.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace con
{
struct Player final
{
	Entity::Hot& _hot;
	Entity::Cold& _cold;

	// Can be a macro, actually.
	Player( Entity::Hot& hot, Entity::Cold& cold ) :
		_hot( hot ),
		_cold( cold )
	{
		_hot.data = this;
	}

	void initialize()
	{
		auto& resources = *Context.prepared_resources;
		constant texture = resources.get_texture( "player"_hcs );
		constant shader = resources.get_shader( "sprite_default"_hcs );

		auto& render_info = _cold.basic_render_info;

		render_info = Render_Info{};

		render_info.texture = texture;
		render_info.shader = shader;
		render_info.elements_count = 6; // 6 elemets in ebo for a quad

		_hot.position ={ 200,200 };

		auto& model_mat = render_info.model_mat;
		model_mat = glm::translate( model_mat, v3{ _hot.position, 0 } );
		model_mat = glm::rotate( model_mat, _hot.rotation_z, v3{ 0,0, 1.0f } );

		// Use actual texture dimensions here, gathered from Resource_Loader?
		auto quad = construct_2d_textured_quad( 140, 108 );

		glGenVertexArrays( 1, &render_info.vao );
		glGenBuffers( 1, &render_info.vbo );

		glBindVertexArray( render_info.vao );

		glBindBuffer( GL_ARRAY_BUFFER, render_info.vbo );
		glBufferData( GL_ARRAY_BUFFER, sizeof( quad ), (void*)&quad, GL_STATIC_DRAW );

		// position 
		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof( f32 ), 0 );
		// texture coord
		glEnableVertexAttribArray( 2 );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof( f32 ), (void*)( 2*sizeof( f32 ) ) );

		// Initialize element buffer and use only one for every sprite.
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, Context.renderer->get_quad_ebo() );

		glBindVertexArray( 0 );
	}

	void shutdown()
	{
		glDeleteBuffers( 1, &_cold.basic_render_info.vao );
		glDeleteBuffers( 1, &_cold.basic_render_info.vbo );
	}
};
}