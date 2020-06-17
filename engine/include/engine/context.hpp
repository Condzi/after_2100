#pragma once

namespace con
{
struct Allocator;
struct Entity_Manager;
struct Logger;
}

namespace con::priv
{
struct Context_Data final
{
	Allocator* default_allocator = nullptr;
	Allocator* temporary_allocator = nullptr;
	Entity_Manager* entity_manager = nullptr;
	Logger* logger = nullptr;
};

struct Context_Helper final
{
	inline static Context_Data data{};
};
}

#define Context con::priv::Context_Helper::data