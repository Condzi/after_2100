#pragma once

#include <engine/bitset.hpp>
#include <engine/typedefs.hpp>

namespace con
{
struct Entity_Base final
{
	compile_constant INVALID_ID   = std::numeric_limits<s16>::min();
	compile_constant INVALID_TYPE = std::numeric_limits<s16>::min();

	v2 position;
	s16 u_id = INVALID_ID;
	s16 type = INVALID_TYPE; // Player, Enemy, Background, Planet, Explosion etc...
	Bitset<CON_MAX_ENTITY_FLAGS> flags;
};
}