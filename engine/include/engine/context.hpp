#pragma once

namespace con
{
struct Allocator;
}

namespace con::priv
{
struct Context_Data final
{
	Allocator* default_allocator = nullptr;
	Allocator* temporary_storage_allocator = nullptr;
};

struct Context_Helper final
{
	inline static Context_Data data{};
};
}

#define Context con::priv::Context_Helper::data