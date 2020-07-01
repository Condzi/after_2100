#pragma once

#include <engine/bitset.hpp>
#include <engine/typedefs.hpp>

#include <engine/render_info.hpp>

namespace con
{
// We allow only one class deep inheritance.
// Holds references to the elements of array in Entity_Manager.
struct Entity
{
	compile_constant INVALID_ID    = std::numeric_limits<s16>::min();
	compile_constant INVALID_TYPE  = std::numeric_limits<s16>::min();
	compile_constant INVALID_GROUP = std::numeric_limits<s16>::min();

	struct Hot final
	{
		v2 position;
		v2 velocity;
		void* data = nullptr; // Pointer to the actual implementation / child?
		f32 rotation_z;
		s16 u_id  = INVALID_ID;
		s16 type  = INVALID_TYPE; // Player, Enemy, Background, Planet, Explosion etc...
		s16 group = INVALID_GROUP;
		Bitset<byte> flags;
	} &hot;

	struct Cold final
	{
		// @Incomplete: Add actual data here.
		// Collision_Info collision_info; // hitbox etc
		Render_Info basic_render_info;
	} &cold;

	virtual ~Entity() = default;
};
}