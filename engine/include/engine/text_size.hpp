#pragma once

#include <engine/typedefs.hpp>

namespace con
{
struct Text_Size final
{
	enum _ : s8
	{
		Developer_Console = 0,
		Menu_Button,
		// ... and oher ...

		elements_count
	};
};

// @ToDo: Call it on window resize.
void update_text_sizes();
}