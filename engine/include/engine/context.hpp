#pragma once

#include <engine/time.hpp>
#include <engine/text_size.hpp>

namespace con
{
struct Allocator;
struct Entity_Manager;
class Logger;
class Config_File;
class Input;
class Prepared_Resources;
class Renderer;
struct Window;
}

namespace con::priv
{
struct Context_Data final
{
	Time_Period const startup_time_point = Time::now();

	Allocator* default_allocator   = nullptr;
	Allocator* temporary_allocator = nullptr;
	Allocator* c_allocator		   = nullptr;
	Allocator* stack_allocator	   = nullptr;
	Entity_Manager* entity_manager = nullptr;
	Logger* logger				   = nullptr;
	Config_File* config_file	   = nullptr;
	Input* input				   = nullptr;
	Prepared_Resources* prepared_resources = nullptr;
	Renderer* renderer			   = nullptr;
	Window* window                 = nullptr;

	struct
	{
		bool requested_by_user = false;
		bool requested_by_app  = false;
	} exit_flags;

	struct
	{
		s32 max_texture_size = -1;
	} machine_info;

	// Shouldn't be accessed directly. Used by construct_text function or
	// something like that.
	s8 text_sizes[Text_Size::elements_count] ={ 0 };
};

struct Context_Helper final
{
	inline static Context_Data data{};
};
}

#define Context con::priv::Context_Helper::data