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
		Array<Entity::Hot>  _hot;
		Array<Entity::Cold> _cold;

		// Only one in entire game, so we let ourself have it like that.
		Player* player = nullptr;
	} by_type;


	void initialize();
	void shutdown();

	void physic_update( f32 ups );
	void frame_update( f32 dt );

	template <typename T>
	returning spawn_entity() -> T*;

private:
	returning create_player() -> Player*;
};


//
// Definitions
//

template <typename T>
returning Entity_Manager::spawn_entity() -> T*
{
	if constexpr ( std::is_same_v<T, Player> ) {
		return create_player();
	} else {
		static_assert( false, "Unsupported entity type!" );
	}
}
}
