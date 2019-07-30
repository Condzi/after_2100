/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/

#include "pch.hpp"

#include "debug_flags.hpp"

namespace con::priv
{
void Debug_Flags::enable_all()
{
	draw_areas = draw_paths = draw_missile_shooters = draw_sound_sources = display_fps = true;
}

void Debug_Flags::disable_all()
{
	draw_areas = draw_paths = draw_missile_shooters = draw_sound_sources = display_fps = false;
}

Debug_Flags& Debug_Flags::get_instance()
{
	static Debug_Flags* instance = new Debug_Flags;

	return *instance;
}
}
