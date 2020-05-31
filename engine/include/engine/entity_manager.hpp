#pragma once

#include <engine/macro_config.hpp>
#include <engine/entity.hpp>
#include <engine/array.hpp>

#include <type_traits>

namespace con
{
//
// Entity Types forward declarations with type enum.
//

struct Enemy;
struct Player;

struct Entity_Type
{
	enum _ : s16
	{
		Enemy,
		Player
	};
};

template <typename T>
returning entity_type_to_id() -> s16
{
	if        constexpr ( std::is_same_v<Enemy, T> ) {
		return Entity_Type::Enemy;
	} else if constexpr ( std::is_same_v<Player, T> ) {
		return Entity_Type::Player;
	} else {
		static_assert( false, "Invalid entity type! Have you forget about adding it to the `entity_ty_to_id`?" );
	}
}

// File_Scope?
namespace priv
{
// @Robustness: Is it necessary to have it here? Maybe move it to separate file, with Entity_Type stuff?
// We still have to access this from this file tho, because of templates madness and factory pattern.
// @Robustness: Second thing, do we need `Entity_Manager` pointer? I believe we can just use Context.entity_manager instead.
// We're not planning more than one EM, do we?
returning create_enemy( Entity_Manager* entity_manager ) -> Enemy*;
returning create_player( Entity_Manager* entity_manager ) -> Player*;
}


struct Entity_Manager final
{
	struct
	{
		Array<Entity::Hot>  _hot{ CON_ENTITIES_COUNT, Context.default_allocator };
		Array<Entity::Cold> _cold{ CON_ENTITIES_COUNT, Context.default_allocator };
	} by_type;
};

}
