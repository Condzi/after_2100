#pragma once

struct Default_Allocator;
struct Temporary_Storage_Allocator;

namespace con::priv
{
struct Context_Data final
{
	Default_Allocator*			 default_allocator = nullptr;
	Temporary_Storage_Allocator* temporary_storage_allocator = nullptr;
};

struct Context_Helper final
{
	inline static Context_Data data{};
};
}

#define Context con::priv::Context_Helper::data