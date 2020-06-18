#pragma once

#include <engine/typedefs.hpp>

namespace con
{
// The values you should default to if reading from config file failes for some reason.
// !!!!!!!!!!!!!! IMPORTANT !!!!!!!!!!!!!!!
// !!									 !!
// !! Please, keep it up to date with    !!
// !! the original local.variables file! !!
// !!									 !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
struct Default_Config_Values final
{
	// @Robustness: Should be in settings file instead? This way we wouldn't have to
	// fail entire program if the settings file would not be found. We could just 
	// recreate it using this values.
	struct display final
	{
		compile_constant vsync = true;
		compile_constant window_size_x = 1280;
		compile_constant window_size_y = 720;
	};

	struct gameplay final
	{
		compile_constant ups = 60;
	};

	struct debug final
	{
		compile_constant dev_console = false;
	};
};
}