/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "input_field.hpp"

namespace con
{
auto Input_Field::get_input_string() const -> std::string const&
{
	return input_string;
}

void Input_Field::draw_gui( Drawing_Set& drawing_set )
{
	if ( not visible ) 
		return;

	drawing_set.add_drawable( background_shape, layer );
}
}