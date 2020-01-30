/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/

#include "pch.hpp"

#include "global_flags.hpp"

namespace con
{
Global_Flags& Global_Flags::get_instance()
{
	static Global_Flags* instance = new Global_Flags;

	return *instance;
}
}