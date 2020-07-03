#include <engine/entity_manager.hpp>

#include <entities/player.hpp>

namespace con
{
void Entity_Manager::initialize()
{
	occupied_hot_cold_slots.initialize( entities_limit, Context.default_allocator );

	by_type._hot.initialize( entities_limit, Context.default_allocator );
	by_type._cold.initialize( entities_limit, Context.default_allocator );

	// Add more types here...
}

void Entity_Manager::shutdown()
{
	// shutdown other entities here as well

	if ( by_type.player ) {
		by_type.player->shutdown();
		Context.default_allocator->free( reinterpret_cast<byte*>( by_type.player ), sizeof( Player ) );
	}

	occupied_hot_cold_slots.shutdown();
	
	by_type._hot.shutdown();
	by_type._cold.shutdown();
}

void Entity_Manager::physic_update( f32 ups )
{
	by_type.player->physic_update( ups );
}

void Entity_Manager::frame_update( f32 dt )
{
	by_type.player->frame_update( dt );
}
}

returning con::priv::create_player( Entity_Manager* entity_manager ) -> Player*
{
	con_assert( entity_manager->by_type.player == nullptr );
	auto* em = entity_manager;
	auto* player = entity_manager->by_type.player;

	player = reinterpret_cast<Player*>( Context.default_allocator->allocate( sizeof( Player ) ) );

	// Find first free slot to use.
	auto idx = em->occupied_hot_cold_slots.find_first_unset_bit().idx;
	// @ToDo: Assert for now, change to con_log later (to fail with style!)
	con_assert( idx != -1 );

	// @Performance: there is an compiler intrinsic that works just like this (it finds
	// first unset bit, changes it to true and returns it position).
	em->occupied_hot_cold_slots.set( idx );
	// @Incomplete: it seems weird. Should we call `new` like that?
	new( player ) Player( em->by_type._hot[idx], em->by_type._cold[idx] );

	player->initialize();

	em->by_type.player = player;

	return player;
}
