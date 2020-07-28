#pragma once

#include <engine/typedefs.hpp>
#include <engine/resource_types.hpp>

namespace con
{
enum class Render_Type : s8
{
	Draw_Elements,
	Draw_Arrays
};

struct Drawing_Group final
{
	enum _ :s16
	{
		Default = 0,
		Ellipse = 1, // Do we use it?
		GUI     = 3
	};
};

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

	Render_Type render_type = Render_Type::Draw_Elements;
	union
	{
		// Maybe ebo should be stored here too?
		// Pass to glDrawElements
		s8 elements_count = -1;

		struct
		{
			s16 vertices_count;
			gl_id mode;
		} draw_arrays_info;
	};

	bool visible = true;
};
}