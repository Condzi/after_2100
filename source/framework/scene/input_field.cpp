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

}