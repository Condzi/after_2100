/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

namespace con::priv
{
// @ToDo (ideas for flags):
// DRAW_PARTICLES
// DRAW_FPS
//
// Maybe use bitset? But only if there'll be many more flags. 
class Debug_Flags final
{
public:
	bool draw_areas{ false };
	bool draw_paths{ false };

	void enable_all();
	void disable_all();

	static Debug_Flags& get_instance();
};
}

#define G_Debug_Flags con::priv::Debug_Flags::get_instance()