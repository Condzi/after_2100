#include <engine/entity_manager.hpp>
#include <engine/algorithms.hpp>

#include <entities/player.hpp>
#include <entities/planet.hpp>

namespace con
{
void Entity_Manager::initialize()
{
	occupied_hot_cold_slots.initialize( entities_limit, Context.default_allocator );

	by_type._hot.initialize( entities_limit, Context.default_allocator );
	by_type._cold.initialize( entities_limit, Context.default_allocator );

	by_type.planet.initialize( CON_PLANETS_LIMIT, Context.default_allocator );
	memset( by_type.planet.data(), 0, sizeof( Planet ) * CON_PLANETS_LIMIT );
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
	by_type.planet.shutdown();
}

void Entity_Manager::physic_update( f32 ups )
{
	by_type.player->physic_update( ups );
}

void Entity_Manager::frame_update( f32 dt )
{
	by_type.player->frame_update( dt );

	for ( s32 i = 0; i < by_type.planet.size(); ++i ) {
		if ( by_type.planet[i] != nullptr ) {
			by_type.planet[i]->frame_update( dt );
		}
	}
}

returning Entity_Manager::create_player() -> Player*
{
	con_assert( by_type.player == nullptr );

	auto player = reinterpret_cast<Player*>( Context.default_allocator->allocate( sizeof( Player ) ) );

	// Find first free slot to use.
	auto idx = occupied_hot_cold_slots.find_first_unset_bit().idx;
	// @ToDo: Assert for now, change to con_log later (to fail with style!)
	con_assert( idx != -1 );

	// @Performance: there is an compiler intrinsic that works just like this (it finds
	// first unset bit, changes it to true and returns it position).
	occupied_hot_cold_slots.set( idx );
	// @Incomplete: it seems weird. Should we call `new` like that?
	new( player ) Player( by_type._hot[idx], by_type._cold[idx] );

	player->initialize();

	by_type.player = player;

	return player;
}

returning Entity_Manager::create_planet( Planet_Resource_Data const& planet_resource_data ) -> Planet*
{
	// Find free slot in planet array.
	constant free_planet_result = linear_find_if( by_type.planet, []( Planet const* planet_ptr ) {
		return planet_ptr == nullptr;
	} );

	// @ToDo: change to better log later.
	con_assert( free_planet_result.found() );

	auto planet = reinterpret_cast<Planet*>( Context.default_allocator->allocate( sizeof( Planet ) ) );

	by_type.planet[free_planet_result.idx] = planet;

	// Find first free _hot / _cold slot to use.
	auto idx = occupied_hot_cold_slots.find_first_unset_bit().idx;
	// @ToDo: Assert for now, change to con_log later (to fail with style!)
	con_assert( idx != -1 );

	occupied_hot_cold_slots.set( idx );
	// @Incomplete: it seems weird. Should we call `new` like that?
	new( planet ) Planet( by_type._hot[idx], by_type._cold[idx] );

	planet->initialize( planet_resource_data );

	return planet;
}
}
