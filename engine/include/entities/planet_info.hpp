#pragma once

#include <engine/typedefs.hpp>

namespace con
{
// Basic info about the planet which is needed for simulating
// orbiting.
struct Planet_Info final
{
	v2 position; // center
	f32 radius; // in pixels; we can't go into lower orbit than radius * 1.1 (or other constant, maybe move it to macro_config?)
};
}