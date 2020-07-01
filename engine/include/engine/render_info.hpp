#pragma once

#include <engine/typedefs.hpp>
#include <engine/resource_types.hpp>

namespace con
{
struct Render_Info final
{
	mat4 combined_matrix; // projection_mat * view_mat * model_mat

	gl_id vao = 0;
	gl_id vbo = 0;
	gl_id ebo = 0;

	Texture texture;
	Shader shader;

	s16 drawing_layer;
	s16 drawing_group;

	// Pass to glDrawElements
	s8 elements_count;

	bool visible = true;
};
}