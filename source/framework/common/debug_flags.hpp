/*
	Conrad 'Condzi' Kubacki 2019
	https://github.com/condzi
*/
#pragma once

#include "stl_extensions.hpp"

namespace con::priv
{
class Debug_Flags final
{
public:
	void enable_all();
	void disable_all();

	[[nodiscard]] auto get( std::string_view name ) -> bool&;
	[[nodiscard]] auto get_flags_names()            -> std::vector<std::string>;
	void toggle( std::string_view name );

	static Debug_Flags& get_instance();

private:
	struct Flag
	{
		std::string_view const name;
		size_t const           hash{ type_hash( name ) };
		bool                   status{ false };
	};

	std::vector<Flag> flags =
	{
		{ "draw_areas"            },
		{ "draw_paths"            },
		{ "draw_missile_shooters" },
		{ "draw_audio_sources"    },
		{ "display_fps"           },
		{ "display_debug_console" }
	};
};
}

#define G_Debug_Flags con::priv::Debug_Flags::get_instance()