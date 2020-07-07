#pragma once

#include <engine/typedefs.hpp>
#include <engine/resource_types.hpp>

namespace con
{
struct Render_Info final
{
//	mat4 combined_matrix; // projection_mat * view_mat * model_mat
	mat4 model_mat{ 1.0f };

	gl_id vao = 0;
	gl_id vbo = 0;
	gl_id ebo = 0;

	Texture texture;
	Shader shader;

	s16 drawing_layer = 0;
	s16 drawing_group = 0;

	// Pass to glDrawElements
	// @ToDo: Better rendering support.
	// We may want to draw vertices, not elements (see: instanciated rendering, particles rendering
	// or just text rendering). Therefore we may want to use an union to hold also `s16 vertices_count` and an `bool render_elements` (or maybe enum?). If true, render as
	// sprites -- using glDrawElements, if false - draw "traditional" way, glDrawArrays or
	// however was that thing called.
	s8 elements_count = -1;

	bool visible = true;
};
}