#pragma once

#include <engine/entity.hpp>
#include <engine/prepared_resources.hpp>
#include <engine/resource_types.hpp>
#include <engine/context.hpp>
#include <engine/logger.hpp>

#include <engine/renderer.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace con
{
struct Debug_Entity final
{
	ENTITY_DEF( Debug_Entity );
	
	void initialize()
	{

	}

	void shutdown()
	{

	}
};
}