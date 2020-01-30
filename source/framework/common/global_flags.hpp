/*
	Conrad 'Condzi' Kubacki 2020
	https://github.com/condzi
*/
#pragma once

#include <bitset>

#include "typedefs.hpp"

namespace con
{
struct Global_Flags final
{
	compile_constant FLAGS_COUNT = u8( 8 );
	std::bitset<FLAGS_COUNT> flags;

	static Global_Flags& get_instance();
};
}

#define G_Flags con::Global_Flags::get_instance().flags