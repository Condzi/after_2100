#include <engine/entity_manager.hpp>

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
	occupied_hot_cold_slots.shutdown();

	by_type._hot.shutdown();
	by_type._cold.shutdown();
}
}