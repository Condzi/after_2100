#include <engine/renderer.hpp>
#include <engine/logger.hpp>
#include <engine/entity_manager.hpp>
#include <engine/algorithms.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// min, max
#include <algorithm>

namespace con
{

void Renderer::initialize()
{
	static constexpr u32 quad_ebo_indecies[] ={
		0, 1, 3,
		1, 3, 2
	};

	glGenBuffers( 1, &quad_ebo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, quad_ebo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof( u32 ), quad_ebo_indecies, GL_STATIC_DRAW );
}

void Renderer::shutdown()
{
	glDeleteBuffers( 1, &quad_ebo );
}

returning Renderer::get_quad_ebo() const -> gl_id const
{
	return quad_ebo;
}

void Renderer::set_window_size( s16 width, s16 height )
{
	con_assert( width > 0 );
	con_assert( height > 0 );

	constant w = static_cast<f32>( width );
	constant h = static_cast<f32>( height );

	compile_constant left = 0.0f;
	compile_constant top  = 0.0f;
	compile_constant near = -1.0f;
	compile_constant far  = 1.0f;

	projection_matrix = glm::ortho( left, w, h, top, near, far );

	update_projection_view_multiplied_matrix = true;
}

void Renderer::render()
{
	//
	// 0. update projection_view_multiplied_matrix if needed
	//

	if ( update_projection_view_multiplied_matrix ) {
		update_projection_view_multiplied_matrix = false;

		projection_view_multiplied_matrix = mat4{ 1 };
		projection_view_multiplied_matrix = projection_matrix /* * view_matrix */;
	}

	//
	// 1. get render infos
	//

	constant& entities = *Context.entity_manager;

	// We can shrink it later if needed. Right now it's rough approximation.
	constant render_infos_max_count = entities.occupied_hot_cold_slots.count_set_bits();

	// No stuff to render. Will this be actually possible in normal game?
	// @Robustness: remove in release builds?
	if ( render_infos_max_count == 0 ) {
		return;
	}

	Array<Render_Info> render_infos;
	render_infos.initialize( render_infos_max_count, Context.temporary_allocator );

	// Also, count of the gathered render infos.
	s32 idx_in_render_infos = 0;
	constant& colds = entities.by_type._cold;

	// We can't have more than render_infos_max_count, so stop if we reach that value.
	for ( s32 i = 0; i < colds.size() && idx_in_render_infos < render_infos_max_count; ++i ) {
		if ( entities.occupied_hot_cold_slots.test( i ) == false ) {
			continue;
		}

		constant& cold = colds[i];

		if ( cold.basic_render_info.visible ) {
			render_infos[idx_in_render_infos] = cold.basic_render_info;
			++idx_in_render_infos;
		}
	}

	if ( idx_in_render_infos < render_infos_max_count ) {
		render_infos.shrink( idx_in_render_infos );
	}

	if ( render_infos.size() <= 0 ) {
		return;
	}


	//
	// 2. sort by group and by layer.
	//

	if ( render_infos.size() > 1 ) {
		// We don't have that many data to sort so it shouldn't matter which algorithm we'll use.
		insertion_sort( render_infos, []( Render_Info const& a, Render_Info const& b ) {
			if ( a.drawing_group == b.drawing_group ) {
				return a.drawing_layer > b.drawing_layer;
			}

			return a.drawing_group > b.drawing_group;
		}  );
	}

	//
	// 3. Draw.
	//

	gl_id current_texture = render_infos[0].texture.id;
	gl_id current_shader  = render_infos[0].shader.id;
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, current_texture );
	glUseProgram( current_shader );

	for ( s32 i = 0; i < render_infos.size(); ++i ) {
		constant& render_info = render_infos[i];

		if ( current_texture != render_info.texture.id ) {
			// We don't want to do this for ellipse.
			if ( render_info.drawing_group == Drawing_Group::Default ) {
				current_texture = render_info.texture.id;

				glActiveTexture( GL_TEXTURE0 );
				glBindTexture( GL_TEXTURE_2D, current_texture );
			}
		}

		if ( current_shader != render_info.shader.id ) {
			current_shader = render_info.shader.id;

			glUseProgram( current_shader );
		}


		// @Performance: don't recompute this matrix every time!
		mat4 mvp_mat{ 1.0f };
		mvp_mat = projection_view_multiplied_matrix * render_info.model_mat;

		// @Robustness: different shaders may require different uniforms to set. Use
		// some if statements to determine what shader is being used and then set the data
		// accordingly.
		constant combined_mat_loc = glGetUniformLocation( current_shader, "u_mvp_mat" );
		glUniformMatrix4fv( combined_mat_loc, 1, GL_FALSE, glm::value_ptr( mvp_mat ) );

		glBindVertexArray( render_info.vao );

		if ( render_info.render_type == Render_Type::Draw_Elements ) {
			glDrawElements( GL_TRIANGLES, render_info.elements_count, GL_UNSIGNED_INT, 0 );
		} else {
			glDrawArrays( render_info.draw_arrays_info.mode, 0, render_info.draw_arrays_info.vertices_count );
		}

		glBindVertexArray( 0 );
	}
}

returning construct_2d_textured_quad( s32 width, s32 height ) -> Array<Textured_Vertex2D>
{
	if ( width <= 0 ) {
		con_log_indented( 2, R"(Warning: invalid parameter for "construct_2d_textured_quad": width = %, changing to 10.)", width );
		width = 10;
	}

	if ( height <= 0 ) {
		con_log_indented( 2, R"(Warning: invalid parameter for "construct_2d_textured_quad": height = %, changing to 10.)", height );
		height = 10;
	}

	Array<Textured_Vertex2D> vertices;
	vertices.initialize( 4, Context.temporary_allocator );

	constant half_width  = static_cast<f32>( width / 2 );
	constant half_height = static_cast<f32>( height / 2 );

	auto& v0 = vertices[0];
	auto& v1 = vertices[1];
	auto& v2 = vertices[2];
	auto& v3 = vertices[3];

	v0.position ={ 0 - half_width, 0 - half_height };
	v1.position ={ 0 - half_width, 0 + half_height };
	v2.position ={ 0 + half_width, 0 + half_height };
	v3.position ={ 0 + half_width, 0 - half_height };

	v0.texture_point ={ 0,0 };
	v1.texture_point ={ 0,1 };
	v2.texture_point ={ 1,1 };
	v3.texture_point ={ 1,0 };

	return vertices;
}

returning construct_textured_sprite( s32 width, s32 height ) -> Render_Info
{
	Render_Info render_info;

	render_info.elements_count = 6; // 6 elemets in ebo for a quad

	auto quad = construct_2d_textured_quad( width, height );

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

	return render_info;
}

void shutdown_textured_sprite( Render_Info const& render_info )
{
	glDeleteVertexArrays( 1, &render_info.vao );
	glDeleteBuffers( 1, &render_info.vbo );
}

}