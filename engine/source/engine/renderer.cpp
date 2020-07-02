#include <engine/renderer.hpp>
#include <engine/logger.hpp>

namespace con
{
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
	v3.texture_point ={ 0,1 };

	return vertices;
}

void Renderer::initialize()
{
	static constexpr u32 quad_ebo_indecies[] ={
		0, 1, 3,
		1, 3, 2
	};

	glGenBuffers( 1, &quad_ebo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, quad_ebo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6, quad_ebo_indecies, GL_STATIC_DRAW );

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

	// @Incomplete: just update the matrix? update mvp at the rendering stage?
	// or just copy the dimensions AND THEN, in render() update all matrices?
	// At this point we may not need to store projection matrix at all?
}
}