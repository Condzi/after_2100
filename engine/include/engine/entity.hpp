#pragma once

#include <engine/bitset.hpp>
#include <engine/typedefs.hpp>

namespace con
{
struct Entity final
{
	compile_constant INVALID_ID    = std::numeric_limits<s16>::min();
	compile_constant INVALID_TYPE  = std::numeric_limits<s16>::min();
	compile_constant INVALID_GROUP = std::numeric_limits<s16>::min();

	struct Hot final
	{
		v2 position;
		v2 velocity;
		void* data = nullptr;
		f32 rotation_z;
		s16 u_id  = INVALID_ID;
		s16 type  = INVALID_TYPE; // Player, Enemy, Background, Planet, Explosion etc...
		s16 group = INVALID_GROUP;
		Bitset<CON_MAX_ENTITY_FLAGS> flags;
	} &hot;

	struct Cold final
	{
		// @Incomplete: Add actual data here.
		// Collision_Info collision_info; // hitbox etc
		// Drawing_Info drawing_info; // buffers, texture, layer, shaders...
		s32 dummy_;
	} &cold;
};
}