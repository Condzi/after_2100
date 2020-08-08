#pragma once

#include <engine/typedefs.hpp>

// @Robustness: Do we want to include this stuff here? Maybe 
// move it to the cpp file to avoid cross include?
#include <engine/allocator.hpp>
#include <engine/entity_manager.hpp>
#include <engine/logger.hpp>
#include <engine/config_file.hpp>
#include <engine/window.hpp>
#include <engine/input.hpp>
#include <engine/resource_loader.hpp>
#include <engine/prepared_resources.hpp>
#include <engine/resource_loader.hpp>
#include <engine/renderer.hpp>

namespace con
{
class Application final
{
public:
	returning initialize() -> bool;
	void shutdown();

	void run();

private:
	// Context structure stuff.
	Default_Allocator    default_allocator;
	Temporary_Allocator  temporary_allocator;
	C_Allocator          c_allocator;
	Stack_Allocator      stack_allocator;
	Entity_Manager       entity_manager;
	Logger               main_logger;
	Config_File          config_file;
	Window               window;
	Input                input;
	Prepared_Resources   prepared_resources;

	Resource_Loader      resource_loader;
	Renderer             renderer;

	f32 ups = -1;

	void dump_logs_to_file();

	returning set_up_log_folder() -> bool;
	returning check_necessary_paths() const -> bool;
};
}