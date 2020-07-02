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
		static_assert( false, R"(Invalid entity type! Have you forget about adding it to the "entity_type_to_id"?)" );
	}
}

namespace priv
{
// returning create_enemy( Entity_Manager* entity_manager ) -> Enemy*;
returning create_player( Entity_Manager* entity_manager ) -> Player*;
}


struct Entity_Manager final
{
	compile_constant entities_limit = CON_ENTITIES_LIMIT;
	//
	// *Every* entity has _hot and _cold data, therefore we can check if they're
	// alive using a bitset. idx in bitset = idx in _hot, _cold. It doesn't match
	// idx in other arrays!
	//
	Bitset occupied_hot_cold_slots;

	struct
	{
		Array<Entity::Hot>  _hot ;
		Array<Entity::Cold> _cold;
	} by_type;


	void initialize();
	void shutdown();
};

}
