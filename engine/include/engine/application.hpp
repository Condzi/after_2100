#pragma once

#include <engine/typedefs.hpp>

// @Robustness: Do we want to include this stuff here? Maybe 
// move it to the cpp file to avoid cross include?
#include <engine/allocator.hpp>
#include <engine/entity_manager.hpp>
#include <engine/logger.hpp>

namespace con
{
class Application final
{
public:
	returning initialize() -> bool;
	// @Idea: Return codes? Different for exit because failure / crash?
	// Or maybe just check the Context's flags (flags.exit_flags[error_blah])?
	void run();
	void shutdown();

private:
	Default_Allocator default_allocator;
	Temporary_Allocator temporary_allocator;
	Entity_Manager entity_manager;
	Logger main_logger;
	FILE* main_logger_file = nullptr;
	// @ToDo: game timer here

	// @Idea: Global struct that contains values like that? Or just hold it in the 
	// config file? Or maybe just gather it once in the run() function from the global config file?
	f32 ups = 1.0f / 60; // = default value; @ToDo ! ! ! !

	void flush_logger();

	returning set_up_log_folder() -> bool;
	returning check_necessary_paths() const -> bool;
};
}