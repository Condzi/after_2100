#pragma once

#include <engine/time.hpp>

namespace con
{
struct Allocator;
struct Entity_Manager;
struct Logger;
struct Config_File;
struct Input;
struct Prepared_Resources;
}

namespace con::priv
{
struct Context_Data final
{
	Time_Period const startup_time_point = Time::now();

	Allocator* default_allocator   = nullptr;
	Allocator* temporary_allocator = nullptr;
	Allocator* c_allocator		   = nullptr;
	Entity_Manager* entity_manager = nullptr;
	Logger* logger				   = nullptr;
	Config_File* config_file	   = nullptr;
	Input* input				   = nullptr;
	Prepared_Resources* prepared_resources = nullptr;

	struct
	{
		bool exit : 1       = false;
		bool glfw_error : 1 = false;
		bool gl_error : 1   = false;
	} engine_flags; // @Idea: rename to `exit_flags` to indicate why we are exiting?
};

struct Context_Helper final
{
	inline static Context_Data data{};
};
}

#define Context con::priv::Context_Helper::data