#pragma once

#include <engine/entity.hpp>
#include <engine/cstring.hpp>

#include <schrift/schrift.h>

namespace con
{
struct Font_Test final
{
	ENTITY_DEF( Font_Test );

	compile_constant string_to_display = L"Hello, world! �柿� �Ư�ӣ <3";

	void initialize()
	{

	}

	void shutdown()
	{

	}
};
}