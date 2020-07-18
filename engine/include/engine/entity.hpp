#pragma once

#include <engine/bitset.hpp>
#include <engine/typedefs.hpp>

#include <engine/render_info.hpp>

namespace con
{
struct Entity_Type final
{
	enum _ : s16
	{
		Debug_Entity,
		Player,
		Planet
	};
};

// Are we actually using it?
/*
template <typename T>
returning entity_type_to_id() -> s16
{
	if constexpr ( std::is_same_v<Player, T> ) {
		return Entity_Type::Player;
	} else {
		static_assert( false, R"(Invalid entity type! Have you forget about adding it to the "entity_type_to_id"?)" );
	}
}
*/

// Acts just like a container for entity related types
// and values.
struct Entity final
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
		bool update_model_matrix = false;
		// @ToDo: do we need it?
		Bitset flags;
	};

	struct Cold final
	{
		// Collision_Info collision_info; // hitbox etc
		// @ToDo: just rename to render_info.
		Render_Info basic_render_info;
	};

	Entity() = delete;
};
}

#define ENTITY_DEF( name__ ) \
con::Entity::Hot&  _hot;									\
con::Entity::Cold& _cold;									\
															\
name__( con::Entity::Hot& hot, con::Entity::Cold& cold ) :	\
	_hot( hot ),											\
	_cold( cold )											\
{															\
	_hot  = con::Entity::Hot{};								\
	_cold = con::Entity::Cold{};							\
															\
	_hot.data = this;										\
	_hot.type = con::Entity_Type::name__;					\
}